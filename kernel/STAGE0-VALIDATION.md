# Stage-0 validation receipts

## 2026-09-27 — freestanding ARM init

The stage-0 `initramfs/miro-a1/init.c` source was cross-compiled independently with Clang/LLD as a freestanding ARM Linux executable using the same architecture, warning, no-libc and entry-point constraints encoded in the repository build.

Validation command shape:

```sh
clang --target=armv7a-linux-gnueabi \
    -march=armv7-a -mfloat-abi=soft \
    -Os -ffreestanding -fno-builtin -fno-stack-protector \
    -fomit-frame-pointer -Wall -Wextra -Werror \
    -nostdlib -static -fuse-ld=lld \
    -Wl,-e,_start -Wl,--build-id=none \
    -o init-arm init.c
```

Receipt:

```text
ELF 32-bit LSB executable
Machine: ARM
EABI5
soft-float ABI
statically linked
entry point: 0x20234
size: 3072 bytes
SHA-256: e0e7835e58ccb4f729ec3f9d83dc5af2468ec80423f3b3ce1c234720fe685c97
```

This proves that the freestanding init source is warning-clean and linkable as a static ARM EABI executable without libc. The SHA-256 above identifies this independent Clang validation build only; the canonical stage-0 bundle is built with the pinned AOSP GCC 4.9 toolchain and will have a different hash.

## 2026-10-02 — first complete hosted stage-0 bundle

GitHub Actions run 37017345054 completed successfully from PR #59 head
`909877508de6a827f766744b55d684904fb39596` using the pinned SharkL3 donor and
AOSP GCC 4.9 toolchain. The build produced an ARM zImage, donor/reference device
trees, the freestanding initramfs, resolved kernel configuration, and artifact
manifest.

Receipt:

```text
53bdd4645040fe9fb3b73bf38340e796ccfb38b3db0e8c361392b022cbe18f60  zImage
b02f6c97d943a5ba283cd6480be92f7701015fbbd2f7fb54114f911aa5668816  donor-base-sp9863a.dtb
4d8931acbcfe8a688d805242eea90e98806281966e5ff30bfd699fb312efca23  reference-board-sp9863a-1h10_go_32b-overlay.dtbo
fe925825f5ce8c9fd42884ca699d8028ced2b988e3158eadc787840830b058e3  initramfs.cpio.gz
57d17f29fe11a5505e191c725b5c871a5c4d73db248894ca3c6d21ad7350f22a  kernel.config
f4fe5eafc2d40b2a267f9da03c7c1d14fb50e898b82ed131b969156d348498ac  ARTIFACTS.txt
```

The uploaded `miro-stage0` artifact was artifact ID 11232595948; the uploaded
archive digest was
`d72e2ec6245e5f547774921a9a6c1c4ad86b96e7551af440e16df9057aa67de1`.

This is a compilation and artifact receipt only. It does not establish that the
donor/reference DTB/DTBO match the physical MIRO, does not create a boot image,
and is not a physical-boot or flash-authorization receipt.
