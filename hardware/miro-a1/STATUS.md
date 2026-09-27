# MIRO A1 bring-up status

Updated 2026-09-27.

This file is a consolidated status index. Detailed evidence and provenance live in the linked hardware/kernel documents. A status such as “source coverage good” means we have credible implementation/documentation donors; it does **not** mean Cheap Phone OS has already booted that subsystem on the physical phone.

## Physical identity and boot layout

Confirmed directly on the stock MIRO A1:

```text
ro.board.platform = sp9863a
ro.hardware = s9863a1h10_go_32b
ro.boot.hardware = s9863a1h10_go_32b
```

The stock phone exposes eMMC, A/B SPL, U-Boot, TrustOS/SML, boot/init_boot/vendor_boot, DTB/DTBO, vbmeta, modem/DSP firmware and related partitions. See [README.md](README.md).

## Current map

| Area | Status | Evidence / next gap |
| --- | --- | --- |
| CPU architecture | source coverage very good | SC9863A Cortex-A55/ARMv8 descriptions located; exact public MIRO-family BSP selects an ARM32 kernel target. Upstream 5.15 SC9863A board support is ARM64, so first compilation uses the public ARM32 SharkL3 vendor donor while 5.15 remains the forward-port reference |
| interrupt architecture | source coverage good | ARM GIC + SC9863A/SharkL3 DTS support located |
| UART | source coverage very good | Spreadtrum UART driver and SC9863A/SharkL3 nodes located |
| I2C controller | source coverage good | Spreadtrum/SharkL3 I2C implementation and bus nodes located |
| SPI controller | source coverage good | Spreadtrum SPI implementation and SharkL3 nodes located |
| GPIO/EIC | source coverage good | Spreadtrum GPIO/EIC implementations located |
| DMA | source coverage good | Spreadtrum DMA implementation and SharkL3 node located |
| SC9863A clocks | source coverage good | SC9863A clock driver and clock bindings located |
| eMMC / SD / SDIO | source coverage good | Spreadtrum SDHCI support located; stock partition receipt confirms eMMC |
| pin multiplexing | partial | SC9863A pinmap documentation and vendor configuration exist; exact MIRO-wide pin map still needs recovery |
| regulator topology | partial | generic/vendor PMIC support exists; exact MIRO rail ownership still needs mapping |
| reset topology | partial | reset controls appear in vendor DTS; exact MIRO board-level reset map remains incomplete |
| USB controller / PHY | partial | SharkL3 vendor source identifies MUSB/USB2 PHY mechanisms; physical MIRO configuration/acceptance still needed |
| touchscreen | **mostly mapped** | stock vendor config confirms FocalTech FT5x46, I2C bus/interface 3, address 0x38, 720×1280; matching public 1H10 source supplies controller/clock/pad topology; power rail remains unresolved |
| display / backlight | not yet physically mapped | public 1H10 display examples exist, but actual MIRO panel/wiring has not been established |
| charger / battery | not yet physically mapped | vendor/reference implementations exist; actual MIRO parts/topology still need receipt |
| audio | not yet mapped | vendor framework/source exists; actual codec, routes, DSP and calibration remain to identify |
| Wi-Fi / Bluetooth | not yet mapped | firmware/driver family still needs exact MIRO identification |
| camera | not yet mapped | vendor camera material exists, but sensors, ISP path, calibration and exact MIRO wiring remain unresolved |

## Touchscreen receipt

The stock MIRO's readable `/vendor/etc/sinput/adaptive_ts.conf` confirms:

```text
vendor        focaltech
product       FT5x46
I2C interface 3
I2C bus       3
I2C address   0x38
surface       720 x 1280

secure-input interrupt number 14
secure-input reset number     15
interrupt pin offset          0x58
reset pin offset              0x5c
```

Matching public `s9863a1h10` Linux/BSP material identifies the corresponding reference path as:

```text
I2C3 MMIO       0x70800000
bus frequency   400 kHz
enable clock    CLK_I2C3_EB
I2C clock       CLK_AP_I2C3
source clock    ext_26m
pads            SCL3 / SDA3
Linux IRQ GPIO  AP GPIO 144
Linux reset     AP GPIO 145
```

The secure-input `14/15` and Linux AP-GPIO `144/145` role assignments line up, but the numeric-domain translation is not yet promoted to a general rule. The touchscreen power rail is still unresolved. See [touchscreen.md](touchscreen.md).

## Next evidence priority

The highest-value remaining stock-phone evidence is the **actual merged device tree / DTB+DTBO** under a context that can read it. That should collapse several partial/not-mapped rows at once:

1. exact pin multiplexing;
2. touchscreen power supply;
3. display/panel and backlight;
4. USB controller/PHY settings;
5. charger/battery and regulator topology;
6. sensor buses and GPIOs;
7. portions of audio, Wi-Fi/Bluetooth and camera wiring.

Keep raw images/hashes before decompilation and keep stock-phone evidence distinct from public 1H10 reference material.


## Stage-0 build status

A reproducible non-flashable bring-up bundle is defined in [../../kernel/BRINGUP.md](../../kernel/BRINGUP.md). It builds:

```text
ARM32 zImage
donor-base-sp9863a.dtb
reference-board-sp9863a-1h10_go_32b-overlay.dtbo
tiny static initramfs
SHA-256 receipt
```

The stage-0 donor is pinned to a public Linux 4.14.199 SharkL3 tree because it contains the exact ARM32 board-family build path. Official Linux 5.15.149 remains the cleaner mechanism/forward-port lane.

The bundled DTB/DTBO are explicitly named donor/reference artifacts; the exact-family overlay is not promoted to a physical MIRO board description. A successful CI compile is a **build receipt only**. It is not permission to flash. Boot-image packing waits for recovered MIRO DT evidence, observed boot-header parameters, and a tested recovery path.
