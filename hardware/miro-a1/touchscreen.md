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

against the connected stock MIRO. Record the output before promoting any reference value to a MIRO fact.

## Evidence sources

- Motorola public UNISOC kernel, `arch/arm/boot/dts/sp9863a-1h10_go_32b-overlay.dts`
- Motorola public UNISOC kernel, `arch/arm/boot/dts/sharkl3.dtsi`
- Motorola public UNISOC kernel, `arch/arm/boot/dts/sc9863a.dtsi`
- adaptive touchscreen DT binding, `Documentation/devicetree/bindings/input/touchscreen/adaptive-touchscreen.txt`
