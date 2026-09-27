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

## Kernel build

No successful kernel compilation receipt is recorded here yet. PR #59's GitHub-hosted kernel build has been queued but has not executed. Do not reinterpret the init receipt as a kernel, DTB, DTBO, boot-image, or physical-boot receipt.
