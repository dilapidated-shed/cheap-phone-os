# MIRO C67 model/platform dossier

This is a **model/platform** baseline for the MIRO C67. It is not yet a physical
board receipt.

## Product identity

FCC records identify:

```text
applicant     FOXX Development Inc.
product       Smart Phone
model         C67
FCC ID        2AQRM-C67
brand family  MIRO / FOXXD / AIRVOICE / FOXXD HTH
```

Sources:

- https://fccid.io/2AQRMC67
- https://fcc.report/FCC-ID/2AQRM-C67/7681832.pdf

## Retail/model facts

Current C67 product material lists:

```text
Android          14
SoC               MediaTek Helio G36
RAM               4 GB physical
RAM marketing     8 GB = 4 GB + 4 GB expansion
storage           64 GB
display           6.75 inch, 1600 x 720, 90 Hz
battery           4900 mAh
USB               Type-C
```

Source:

- https://www.newegg.com/miro-c67-6-75-black/p/23B-00MN-00005

## SoC baseline

MediaTek specifies Helio G36 as:

```text
CPU               8 x Arm Cortex-A53
CPU bit width     64-bit
max CPU clock     2.2 GHz
process           12 nm
GPU               IMG PowerVR GE8320
GPU max clock     680 MHz
memory support    LPDDR3 / LPDDR4X
storage support   eMMC 5.1
```

Source:

- https://www.mediatek.com/products/smartphones/mediatek-helio-g36

These are SoC capabilities. They do not prove the exact physical DRAM package,
eMMC package, board wiring or Android ABI used by the MIRO unit.

## Physical facts still missing

Do not start C67 kernel/OS assumptions from the A1 receipt. Retain a C67-specific
physical capture for at least:

```text
ro.board.platform
ro.hardware
ro.boot.hardware
ro.product.cpu.abi
ro.product.cpu.abilist
ro.product.cpu.abilist32
ro.product.cpu.abilist64
kernel machine/version
page size
/dev/block/by-name map
boot-slot structure
DTB / DTBO identity and hashes
MemTotal / zram
internal-storage transport
EGL / GL renderer strings
sensor inventory
```

The Helio G36 CPU being 64-bit does not prove an `arm64-v8a` Android userspace.
Likewise, the A1's `armeabi-v7a` runtime does not prove the C67 is 32-bit.

## Bring-up boundary

Until a physical board receipt exists, keep C67 work at the discovery boundary:

```text
retail/FCC identity
  -> physical Android/boot/ABI inventory
  -> recover DTB/DTBO and boot layout
  -> identify public/vendor source families
  -> only then choose a kernel/device-tree bring-up path
```

Cheap Phone OS issue #22 already requires A1 and C67 results to remain separate
in the physical-phone acceptance matrix.

Shared compiler/NDK hardware summaries are mirrored under
`isomorphisms/android-NDK/hardware/`.
