# MIRO A1 physical hardware receipt

Observed on the physical MIRO A1 on 2026-09-26.

## Platform identity

```text
ro.board.platform = sp9863a
ro.hardware = s9863a1h10_go_32b
ro.boot.hardware = s9863a1h10_go_32b
```

This confirms the Spreadtrum/UNISOC SC9863A / SharkL3 / 1H10 platform family. It does not prove that the MIRO board is electrically identical to the upstream SP9863A-1H10 reference board.

## Boot/storage structure

The phone exposes eMMC and an A/B boot structure. Relevant names observed under `/dev/block/by-name`:

```text
spl_a          -> /dev/block/mmcblk0boot0
spl_b          -> /dev/block/mmcblk0boot1

trustos_a      trustos_b
sml_a          sml_b
uboot_a        uboot_b
uboot_log
pm_sys_a       pm_sys_b
teecfg_a       teecfg_b
hypervsior_a   hypervsior_b

boot_a         boot_b
vendor_boot_a  vendor_boot_b
init_boot_a    init_boot_b
dtb_a          dtb_b
dtbo_a         dtbo_b
vbmeta_a       vbmeta_b
```

The modem/DSP firmware is separately partitioned, including:

```text
l_modem_a      l_modem_b
l_gdsp_a       l_gdsp_b
l_ldsp_a       l_ldsp_b
l_fixnv1_a/b
l_fixnv2_a/b
l_deltanv_a/b
l_runtimenv1
l_runtimenv2
```

Initial cheap-phone bring-up should leave these modem/DSP firmware partitions alone.

## First kernel boundary

Initial replacement work starts at the Linux/device-tree/userspace boundary while preserving the existing Boot ROM, SPL, secure firmware, U-Boot and modem firmware until there is a reason to replace them.

Target first boot:

```text
vendor Boot ROM / SPL / early firmware / U-Boot
    -> cheap-phone Linux kernel
    -> MIRO device tree
    -> tiny initramfs
    -> console
```

The first device-support tranche is UART, I2C, SPI, GPIO/EIC, DMA, clocks, regulators/reset infrastructure, eMMC/SD and USB plumbing. Display, touch, battery/charging, sensors, audio, radio, Wi-Fi/Bluetooth and cameras follow only with board-specific evidence.


## Public source match for this target

The exact target name `s9863a1h10_go_32b` appears in public UNISOC-derived source trees, including:

- `jingpad-bsp/device_sprd_sharkl3/s9863a1h10_go_32b/`
- Motorola's public UNISOC kernel overlay `sp9863a-1h10_go_32b-overlay.dts`
- additional SharkL3 board configuration in public Spreadtrum-derived kernel trees.

This establishes a much stronger reference baseline than the generic mainline 1H10 board. It still does not establish that any one public overlay is the MIRO overlay. Compare it against the MIRO's raw `dtb_a` and `dtbo_a`.

See `../sc9863a/README.md` for the documentation and BSP source map.
