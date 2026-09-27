# Kernel foundation

The cheap-phone operating system does not inherit Android's userspace architecture by default. Linux is the initial hardware-support kernel because it already contains useful Spreadtrum/UNISOC mechanisms for the SC9863A family.

## Kernel lanes

The physical MIRO A1 reports a `5.15.149-android13-8-...` kernel, so official Linux `v5.15.149` remains the preferred clean reference and forward-port donor. However, upstream SC9863A board support in that tree is under `arch/arm64`, while the exact public `s9863a1h10_go_32b` BSP explicitly selects `BSP_BOARD_ARCH="arm"` and `sprd_sharkl3_defconfig`.

For the **first buildable stage-0 bundle**, use the pinned public Motorola/Spreadtrum-derived Linux 4.14.199 tree documented in [BRINGUP.md](BRINGUP.md). It contains the ARM32 SharkL3 defconfig and the exact `sp9863a-1h10_go_32b` board-family overlay. This is a bring-up donor, not the long-term kernel decision.

Reference/forward-port lane:

```sh
sh _/sync-kernel
```

First ARM32 stage-0 build lane:

```sh
sh _/sync-miro-stage0-kernel
sh _/build-miro-stage0
```

Both checkouts live under `_/` and are intentionally not flattened into this repository. Keep source provenance and licenses with each upstream tree.

## Already useful upstream mechanisms

The Linux 5.15 source family contains these Spreadtrum/UNISOC implementations that are relevant to the SC9863A bring-up:

| Function | Linux source |
| --- | --- |
| UART | `drivers/tty/serial/sprd_serial.c` |
| I2C | `drivers/i2c/busses/i2c-sprd.c` |
| SPI | `drivers/spi/spi-sprd.c` |
| DMA | `drivers/dma/sprd-dma.c` |
| GPIO | `drivers/gpio/gpio-sprd.c` |
| external interrupt GPIO | `drivers/gpio/gpio-eic-sprd.c` |
| SC9863A clocks | `drivers/clk/sprd/sc9863a-clk.c` |
| eMMC/SD/SDIO host | `drivers/mmc/host/sdhci-sprd.c` |
| SC9863A SoC description | `arch/arm64/boot/dts/sprd/sc9863a.dtsi` |
| SharkL3 platform description | `arch/arm64/boot/dts/sprd/sharkl3.dtsi` |
| 1H10 reference board | `arch/arm64/boot/dts/sprd/sp9863a-1h10.dts` |

Use these implementations before inventing replacements. Replace an inherited interface only when doing so makes the cheap-phone system clearer, smaller, safer, or easier to control.

## What is not yet claimed

The physical MIRO board wiring is not the generic 1H10 reference board merely because the platform family matches. In particular, pin control, USB controller/PHY details, display, touch, audio, Wi-Fi/Bluetooth, charging, sensors, modem transport and cameras still require the MIRO device tree, vendor kernel/configuration, or physical probing.

Do not enable an SC9860-specific pinctrl driver merely because the name is nearby. Import the actual MIRO/SC9863A implementation when identified.

## Interface naming

Kernel implementation names do not dictate the user-facing vocabulary. The shell and system API may expose readable names such as `list`, `copy`, `kernel_messages`, `end_process`, `halt_process`, `resume_process`, `emergency_stop_process`, and `talk_to_process`, with traditional short spellings retained only as aliases where useful.
