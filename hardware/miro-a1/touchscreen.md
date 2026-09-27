# MIRO A1 touchscreen bring-up

## Exact-target public reference

A public vendor-derived `sp9863a-1h10_go_32b-overlay.dts` for the same UNISOC board target reported by the MIRO contains:

```dts
&i2c3 {
    status = "okay";

    touchscreen@38 {
        compatible = "adaptive-touchscreen";
        reg = <0x38>;
        gpios = <&ap_gpio 145 GPIO_ACTIVE_HIGH
                 &ap_gpio 144 GPIO_ACTIVE_HIGH>;
        controller = "focaltech,FT5436";
    };
};
```

The adaptive-touchscreen binding defines the two GPIOs in order as **reset, interrupt**. Thus the reference target says:

```text
bus          i2c3
controller   FocalTech FT5436
address      0x38
reset GPIO   AP GPIO 145
IRQ GPIO     AP GPIO 144
```

The same overlay maps the relevant pads:

```text
SHARKL3_EXTINT0 -> GPIO 144
SHARKL3_EXTINT1 -> GPIO 145
SHARKL3_SCL3    -> GPIO 146
SHARKL3_SDA3    -> GPIO 147
```

The vendor SharkL3 DTS defines I2C3 as:

```text
MMIO address    0x70800000
IRQ             GIC SPI 14
bus frequency   400000 Hz
enable clock    CLK_I2C3_EB
I2C clock       CLK_AP_I2C3
source clock    ext_26m
reset           MASK_AP_APB_I2C3_SOFT_RST
```

No `avdd-supply` appears in this public reference touchscreen node, even though the adaptive-touchscreen binding supports one. That means the reference source alone does **not** tell us which physical regulator powers the touchscreen.

These values are a hypothesis for the MIRO A1, not a physical receipt. Run:

```sh
sh _/inspect-miro-touchscreen
```

directly in Termux on the stock MIRO. The script reads the running phone's live device tree and sysfs locally. Use `--adb` only when deliberately probing a different connected Android device. Record the output before promoting any reference value to a MIRO fact.

## Evidence sources

- Motorola public UNISOC kernel, `arch/arm/boot/dts/sp9863a-1h10_go_32b-overlay.dts`
- Motorola public UNISOC kernel, `arch/arm/boot/dts/sharkl3.dtsi`
- Motorola public UNISOC kernel, `arch/arm/boot/dts/sc9863a.dtsi`
- adaptive touchscreen DT binding, `Documentation/devicetree/bindings/input/touchscreen/adaptive-touchscreen.txt`


## Physical MIRO Termux receipt — 2026-09-27

Running `_/inspect-miro-touchscreen` directly in Termux on the stock MIRO A1 produced:

```text
Platform:
  sp9863a
  s9863a1h10_go_32b
  s9863a1h10_go_32b

I2C devices:
  none visible to the Termux app UID

Touchscreen DT node:
  none visible to the Termux app UID

Accessible vendor files:
  /vendor_dlkm/lib/modules/focaltech_ats.ko
  /vendor_dlkm/lib/modules/il79451a_touch_spi.ko
  /vendor/etc/sinput/adaptive_ts.conf
```

The app UID was also denied access to `/proc/modules`, `/proc/bus/input/devices`, InputManager's `dumpsys`, and the input-event sysfs inventory.

Interpretation:

- absence of the live DT/I2C/input entries in this receipt is an Android sandbox visibility limit, not evidence that those kernel objects are absent;
- `focaltech_ats.ko` is strong corroboration for the exact-target SC9863A reference, which uses FocalTech FT5436 over I2C3;
- `il79451a_touch_spi.ko` proves the stock vendor image also carries an Ilitek SPI touchscreen option, so module presence alone cannot identify the installed MIRO panel;
- the selected controller still needs to be established from accessible vendor configuration/module-load metadata or a higher-privilege shell/device-tree receipt.


## Physical MIRO adaptive-touch configuration — 2026-09-27

The stock MIRO's readable `/vendor/etc/sinput/adaptive_ts.conf` contains:

```text
int_pin_offset   0x58
rst_pin_offset   0x5c
int_gpio_num     14
rst_gpio_num     15
pin_fun_mask     0x30
int_fun_ns       3
int_fun_se       2
rst_fun_ns       3
rst_fun_se       2
spi_max_speed_hz 0
width            720
height           1280
i2c_intf         3
i2c_bus          3
i2c_addr         0x38
spi_intf         0
spi_bus          0
spi_chip_select  0
spi_mode         0
spi_bits_per_word 0
vendor           focaltech
product          FT5x46
```

This physically observed stock configuration confirms that this MIRO build is configured for a **FocalTech FT5x46-family touchscreen over I2C bus 3 at address 0x38**, not the alternate Ilitek SPI path also shipped in the vendor module set.

The same values through the I2C/controller fields are present in public UNISOC-derived `s9863a1h10/sinput_conf/stp.conf` sources. That is a direct match between the stock MIRO configuration and the public SC9863A 1H10 BSP target.

### GPIO numbering

The secure-input configuration names:

```text
interrupt GPIO 14
reset GPIO     15
```

while the older Linux board overlay for the same target names:

```text
reset     AP GPIO 145
interrupt AP GPIO 144
```

and the adaptive-touch Linux binding defines the `gpios` order as reset then interrupt.

The role assignment is therefore consistent across both sources. The numeric schemes differ by 130 in this case. Do not yet treat `14 -> 144` and `15 -> 145` as a proven general GPIO-number translation rule; preserve both numbering domains until the secure-input GPIO mapping source or the MIRO's merged DT is recovered.

### Current touchscreen map

```text
controller family   FocalTech FT5x46       confirmed from stock vendor config
transport           I2C                    confirmed from stock vendor config
I2C interface       3                      confirmed from stock vendor config
I2C bus             3                      confirmed from stock vendor config
I2C address         0x38                   confirmed from stock vendor config
surface             720 x 1280             confirmed from stock vendor config

Linux reference:
MMIO controller     I2C3 @ 0x70800000
bus frequency       400 kHz
enable clock        CLK_I2C3_EB
I2C clock           CLK_AP_I2C3
source clock        ext_26m
SCL/SDA             SCL3 / SDA3

secure-touch config:
interrupt number    14
reset number        15
interrupt pin off   0x58
reset pin off       0x5c

Linux reference:
interrupt           AP GPIO 144
reset               AP GPIO 145

power rail          unresolved
```

The exact physical regulator remains unresolved. The old adaptive-touch DT binding supports an `avdd-supply`, but the public SC9863A 1H10 touchscreen node does not specify one. The driver only explicitly enables a regulator when that property is present, so the reference design may rely on an always-on/shared rail or power managed outside this node. Do not invent the rail.
