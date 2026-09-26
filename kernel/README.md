# Kernel foundation

The cheap-phone operating system does not inherit Android's userspace architecture by default. Linux is the initial hardware-support kernel because it already contains useful Spreadtrum/UNISOC mechanisms for the SC9863A family.

## Starting donor

The working donor is the official Linux 5.15 stable line, initially pinned to `v5.15.149` because the physical MIRO A1 reports a `5.15.149-android13-8-...` kernel. The pin is a compatibility starting point, not a claim that upstream Linux 5.15.149 is the phone's exact vendor kernel.

Run:

```sh
sh _/sync-kernel
```

The checkout is materialized under `_/linux/` and is intentionally not flattened into this repository. Keep source provenance and licenses with the upstream tree.

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
