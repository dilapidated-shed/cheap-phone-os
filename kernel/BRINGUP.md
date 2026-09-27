# MIRO A1 kernel bring-up

## Two kernel lanes

Cheap Phone OS currently keeps two distinct kernel source roles.

### Stage 0: buildable ARM32 SharkL3 donor

The first buildable MIRO bundle uses the public Motorola/Spreadtrum-derived kernel at:

```text
https://github.com/MotorolaMobilityLLC/kernel-sprd
c4e610cd833c47f08969678fa58f9bfcb57bd3a5
```

That source identifies itself as Linux 4.14.199 in `sprd_sharkl3_defconfig`. It is useful because it contains all three of the things needed for an honest first MIRO-family build:

```text
ARCH=arm
sprd_sharkl3_defconfig
arch/arm/boot/dts/sp9863a-1h10_go_32b-overlay.dts
```

The exact public board-family overlay is still reference evidence rather than proof of every MIRO component. It includes reference display, charger, camera and GPIO choices that must not silently become MIRO facts. The bundle therefore names it `reference-board-...dtbo` and does not define it as the physical boot overlay. Stage 0 is a **bring-up donor**, not the intended long-term kernel.

### 5.15.149 reference / forward-port lane

The physical MIRO reports a 5.15.149 Android kernel lineage. Official Linux 5.15.149 contains substantially cleaner SC9863A mechanisms, but its SC9863A board description is under `arch/arm64`. The public exact-target BSP, by contrast, explicitly selects `BSP_BOARD_ARCH="arm"`.

Therefore do not pretend that upstream 5.15.149 can simply be configured as the current MIRO ARM32 target. Keep it as:

- the preferred source for upstreamed SC9863A mechanisms;
- a comparison point against vendor code;
- the forward-port destination once the exact 5.15 vendor delta or enough hardware evidence has been recovered.

## Stage-0 artifact boundary

The stage-0 build produces separate, inspectable pieces. The names deliberately distinguish the donor/reference device trees from recovered physical MIRO data:

```text
zImage
donor-base-sp9863a.dtb
reference-board-sp9863a-1h10_go_32b-overlay.dtbo
initramfs.cpio.gz
SHA256SUMS
```

It deliberately does **not** create a flashable Android `boot.img`.

Before constructing a boot image we still need the actual MIRO boot-image/header parameters, command line, DTB/DTBO selection behavior, and a tested recovery path. A file that merely has a `.img` suffix is not a recovery strategy.

## Initramfs

The first userspace is a tiny statically linked ARM program in `initramfs/miro-a1/init.c`. It:

- mounts devtmpfs, procfs and sysfs;
- attaches to `/dev/console` when available;
- prints an unmistakable Cheap Phone OS stage-0 banner;
- reports `uname`, `/proc/cmdline`, and the live device-tree model/compatible strings when readable;
- stays alive instead of falling through into a kernel panic.

This is intentionally smaller than importing Android init or a general-purpose shell into the first boot proof.

## Touchscreen boundary

The stock MIRO configuration confirms FocalTech FT5x46 on I2C bus 3 at address `0x38`. The public exact-family Linux overlay agrees and supplies the reset/interrupt wiring.

The touchscreen regulator is still not known. Stage 0 therefore **builds the exact-family reference DTBO unchanged as evidence**, but Cheap Phone OS does not add a guessed regulator or claim independent touchscreen power sequencing.

The first physical boot does not require the touchscreen to work.

## Build

Fetch the pinned donor and the donor-era AOSP GCC 4.9 toolchain, then build:

```sh
sh _/sync-miro-stage0-kernel
sh _/sync-miro-stage0-toolchain
sh _/build-miro-stage0
```

The vendor kernel and tiny init are both compiled with the pinned AOSP `arm-linux-androideabi-4.9` toolchain. The init is freestanding and uses only the Linux ARM EABI syscall interface: no libc, Android runtime, or dynamic linker is present.

The build is cwd-independent. Defaults:

```text
kernel source  _/miro-stage0-kernel
kernel output  _/out/miro-stage0-kernel
artifacts      _/artifacts/miro-stage0
kernel/init compiler _/toolchains/arm-linux-androideabi-4.9/bin/arm-linux-androideabi-
```

Override them with `MIRO_STAGE0_KERNEL_DIR`, `MIRO_STAGE0_TOOLCHAIN_DIR`, `MIRO_STAGE0_OUT`, `MIRO_STAGE0_ARTIFACTS`, `KERNEL_CROSS_COMPILE` or `JOBS`.

GitHub Actions also runs the same scripts so the phone does not need a compiler installed locally.

## Physical-boot gate

The stock phone can collect the next evidence locally, without ADB or fastboot:

```sh
sh _/inspect-miro-boot
```

The probe is read-only. It records boot properties, current slot information when exposed, named partition links/readability, bootconfig/cmdline visibility, fstab candidates, and live device-tree chosen properties.

Do not flash Stage 0 merely because it compiles. The next gate is:

1. preserve/extract the stock boot inputs;
2. record their hashes and header parameters;
3. establish which boot path can load a temporary or recoverable image;
4. only then package the stage-0 pieces using those observed parameters;
5. boot on the sacrificial MIRO first.

The stock/reference MIRO remains the hardware oracle.
