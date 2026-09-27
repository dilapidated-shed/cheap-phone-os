# SC9863A / SharkL3 source and documentation map

Research sweep: 2026-09-26.

This file records public material useful for bringing up the MIRO A1. The physical MIRO reports:

```text
ro.board.platform = sp9863a
ro.hardware = s9863a1h10_go_32b
ro.boot.hardware = s9863a1h10_go_32b
```

The exact string `s9863a1h10_go_32b` also appears in public UNISOC-derived BSP/device sources. It is therefore a standard SharkL3/SC9863A board target name, not a Foxx-only invention.

## Priority order

Use evidence in this order:

1. actual MIRO DTB/DTBO, kernel configuration, partition contents and physical measurements;
2. exact `s9863a1h10_go_32b` public BSP material;
3. SC9863A/SharkL3 vendor-derived kernel and bootloader material;
4. UNISOC customer/hardware documentation and reference design;
5. upstream Linux SC9863A support.

A reference board or another phone with the same SoC is evidence about the platform, not proof of MIRO wiring.

## Primary / near-primary documentation

| Material | Location | Useful content | Status |
| --- | --- | --- | --- |
| UNISOC SC9863A product page | https://www.unisoc.com/en/product/SmartPhone/9863A | official high-level SoC identity and capabilities | public |
| SC9863/SC9863A documentation index | https://bbs.16rd.com/citiao-xinpian-SC9863.html | index of hardware, pinmap, camera, RF, DDR and application documents | public index; individual attachments vary |
| SC9863A Pinmap configuration guide | https://bbs.16rd.com/thread-571451-1-1.html | pin mux, drive strength, pull state, sleep state, UART/I2C/SPI matrix rules | public article; attachment may require site login/credits |
| SC9863A Android 9 customer configuration | https://bbs.16rd.com/thread-583064-1-1.html | `device/sprd/sharkl3`, `vendor/sprd`, `sprd-board-config`, `sprd-diffconfig`, DTBO, pinmap and U-Boot paths | public article |
| SC9863A Android Q customer configuration | https://bbs.16rd.com/thread-577465-1-1.html | newer BSP layout, modules, DTB/DTBO, pinmap examples | public article; PDF named `SC9863AAndroidQ客户化配置V1.0_nowatermark.pdf` is referenced |
| SC9863A GPIO configuration notes | https://bbs.16rd.com/thread-478048-1-1.html | GPIO mux rules, EIC, LCD/SPI notes, regulator/GPIO debugging, USB-download notes | public article |
| SC9863A reference-design files | https://bbs.16rd.com/thread-644167-1-1.html | listing for `SC9863A.DSN` and `SC9863A.brd`; potentially schematic/PCB-level reference wiring | attachment access restricted by forum mechanism |

The documentation index also advertises hardware design, application, DDR, EMI, RF, SensorHub and camera training material. Do not copy restricted attachments into this repository without checking redistribution rights; record the source and retrieve for local study when permitted.

## Exact public board-target material

### JingPad / UNISOC device tree

Repository:

```text
https://github.com/jingpad-bsp/device_sprd_sharkl3
```

Exact directory:

```text
s9863a1h10_go_32b/
```

Observed files include:

```text
s9863a1h10_go_32b_Base.mk
s9863a1h10_go_32b_2g.mk
s9863a1h10_go_32b_Natv.mk
system.prop
```

The product files identify:

```text
TARGET_BOARD_PLATFORM := sp9863a
TARGET_BOARD := s9863a1h10_go_32b
PRODUCT_GO_DEVICE := true
```

### JingPad full BSP snapshot

Repository:

```text
https://github.com/deadman96385/jingpad_android_bsp
```

Relevant directory:

```text
device/sharkl3/androidq/s9863a1h10_go_32b/
```

Observed configuration includes:

```text
BSP_BOARD_NAME="s9863a1h10_go_32b"
BSP_BOARD_ARCH="arm"
BSP_KERNEL_DEFCONFIG="sprd_sharkl3_defconfig"
```

This is useful as a topology/reference corpus; do not assume its binaries or exact board overlay are the MIRO's.

### Public UNISOC/Spreadtrum kernel trees

```text
https://github.com/strongtz/linux-sprd
https://github.com/MotorolaMobilityLLC/kernel-sprd
https://github.com/turtleletortue/android_kernel_retroid_pocket2plus
```

Useful paths observed across these trees include:

```text
arch/arm/configs/sprd_sharkl3_defconfig
sprd-diffconfig/.../sharkl3/
sprd-board-config/sharkl3/sp9863a_1h10/
arch/arm/boot/dts/sp9863a-1h10_go_32b-overlay.dts
```

The Retroid tree has board-config entries selecting:

```text
KERNEL_DEFCONFIG := sprd_sharkl3_defconfig
TARGET_DTB := sp9863a-1h10
```

Motorola's public UNISOC kernel contains `sp9863a-1h10_go_32b-overlay.dts`, giving another vendor-derived source for the exact board-family target.

### Additional exact-target fragments

```text
https://github.com/coldraintea/SPRD-stuff
```

contains:

```text
sharkl3/s9863a1h10_go_32b/
```

with product makefiles and properties identifying the same target.

## Vendor BSP layout recovered from documentation

The public customer-configuration material points to this older UNISOC layout:

```text
device/sprd/sharkl3/
vendor/sprd/

kernel/
    sprd-board-config/
    sprd-diffconfig/

u-boot15/board/spreadtrum/sp9863a_1h10/
    pinmap-sp9863a.c
```

Other public SC9863A build notes identify paths of the form:

```text
bsp/bootloader/chipram/include/configs/sp9863a_1h10_32b.h

bsp/bootloader/u-boot15/
    arch/arm/dts/sp9863a_1h10_32b.dts
    configs/sp9863a_1h10_32b_defconfig

bsp/kernel/kernel4.14/
    arch/arm/configs/sprd_sharkl3_defconfig
    arch/arm/boot/dts/sp9863a-1h10_go_32b-overlay.dts
```

Treat these path names as a search map for source recovery, not as evidence that the MIRO's Android-14 build still uses the same 4.14 tree.

## Pinmap facts worth preserving

The SC9863A pinmap customer material describes configurable properties including:

- alternate function selection;
- drive strength;
- active pull-up/pull-down;
- strong pull-up;
- subsystem-associated sleep state;
- sleep pull state;
- sleep input/output/high-impedance state;
- UART, SPI, I2C and SIM matrix routing.

It names global/matrix registers including:

```text
REG_PIN_UART_MATRIX_MTX_CFG
REG_PIN_UART_MATRIX_MTX_CFG1
REG_PIN_IIS_MATRIX_MTX_CFG
REG_PIN_SIM_MATRIX_MTX_CFG
REG_PIN_SPI_MATRIX_MTX_CFG
REG_PIN_IIC_MATRIX_MTX_CFG
```

This is why the mainline SC9860 pinctrl implementation must not be substituted blindly: the exact SC9863A pin table and board pin assignments are recoverable from better evidence.

## Mainline Linux reference

Official Linux already contains:

```text
arch/arm64/boot/dts/sprd/sc9863a.dtsi
arch/arm64/boot/dts/sprd/sharkl3.dtsi
arch/arm64/boot/dts/sprd/sp9863a-1h10.dts
drivers/clk/sprd/sc9863a-clk.c
drivers/tty/serial/sprd_serial.c
drivers/i2c/busses/i2c-sprd.c
drivers/spi/spi-sprd.c
drivers/dma/sprd-dma.c
drivers/gpio/gpio-sprd.c
drivers/gpio/gpio-eic-sprd.c
drivers/mmc/host/sdhci-sprd.c
```

The 1H10 reference DTS provides useful address/interrupt/clock evidence, but is intentionally sparse compared with a production phone board description.

## Next physical evidence to acquire

The highest-value next receipt from the MIRO is the actual device-tree material:

```text
dtb_a
dtbo_a
```

Preserve exact hashes before interpretation. Decompile them, keep the raw originals, then compare:

```text
MIRO DTB/DTBO
    vs exact s9863a1h10_go_32b vendor-derived overlays
    vs UNISOC 1H10 BSP
    vs mainline sp9863a-1h10.dts
```

That comparison should identify the actual MIRO display, touch controller, sensors, GPIO assignments, regulators, USB topology and other board-specific differences.
