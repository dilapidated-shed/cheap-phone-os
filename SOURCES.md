# Android source map

Observed 2026-09-17; rechecked 2026-09-18.

This file identifies Android source layers that matter to cheap-phone work. It is a working map, not a replacement for the complete manifest-selected trees.

## Source families

- **AOSP** is the canonical provenance baseline. `android-latest-release` currently selects `android17-release`.
- **CyanogenMod** is the historical predecessor of LineageOS and remains a distinct source corpus. Preserve its manifest plus characteristic build, framework, and vendor/configuration forks rather than treating LineageOS as a substitute for the older history.
- **LineageOS** supplies broad aftermarket device support. Its controlling manifest is `LineageOS/android`; the current default branch is `lineage-23.2`.
- **GrapheneOS** supplies a hardened downstream and distinct security-oriented implementations. Its controlling manifest is `GrapheneOS/platform_manifest`; the current default branch is `17`.
- The mirror inventory also seeds **CalyxOS**, **crDroid**, **BlissOS**, **Android-x86**, **/e/OS**, and the historical **DivestOS** build corpus. See `MIRRORS.tsv`.
- This list is a starting set, not an exclusion list. Add other public Android implementations, device trees, kernels, ports, and historical source when they contain a technically distinct answer worth preserving.
- A downstream manifest may use its own fork for one component and AOSP directly for another. Always follow the manifest rather than assuming every path has a downstream fork.

## First-wave components

| AOSP checkout path / repository | Why it matters here | LineageOS | GrapheneOS |
| --- | --- | --- | --- |
| `platform/manifest` | Manifest repository: complete source inventory and revisions; `repo` stores it under `.repo/manifests` rather than as a normal source checkout path | `LineageOS/android` | `GrapheneOS/platform_manifest` |
| `build/make` | Product definitions, Android Go defaults, image composition | `LineageOS/android_build` | `GrapheneOS/platform_build` |
| `build/soong` | Build graph and Android.bp machinery | `LineageOS/android_build_soong` | `GrapheneOS/platform_build_soong` |
| `system/core` | init, properties, process groups, fastboot plumbing, early boot | `LineageOS/android_system_core` | `GrapheneOS/platform_system_core` |
| `packages/modules/adb` | Android Debug Bridge client/daemon implementation | `LineageOS/android_packages_modules_adb` | `GrapheneOS/platform_packages_modules_adb` |
| `system/fs/fs_mgr` | Android 17 mount/fstab/verity/overlayfs code; directly relevant to new filesystems | In `android_system_core/fs_mgr` on `lineage-23.2`; follow the Lineage manifest as this split changes | `GrapheneOS/platform_system_fs_fs_mgr` |
| `system/vold` | Removable/adoptable storage and volume lifecycle | `LineageOS/android_system_vold` | `GrapheneOS/platform_system_vold` |
| `system/memory/lmkd` | Low-memory pressure policy; directly relevant to Android Go behavior | Current `lineage-23.2` manifest uses AOSP directly | Current GrapheneOS 17 manifest uses AOSP directly |
| `frameworks/native` | Binder-native plumbing, SurfaceFlinger, native graphics/services | `LineageOS/android_frameworks_native` | `GrapheneOS/platform_frameworks_native` |
| `frameworks/base` | system_server, ActivityManager, package/window management, low-RAM framework behavior | `LineageOS/android_frameworks_base` | `GrapheneOS/platform_frameworks_base` |
| `art` | DEX/ART execution, verification, compilation, runtime | `LineageOS/android_art` | `GrapheneOS/platform_art` |
| `bionic` | libc, dynamic linker, native process boundary | `LineageOS/android_bionic` | `GrapheneOS/platform_bionic` |
| `libcore` | Java core libraries used by ART/framework code | Current `lineage-23.2` manifest uses AOSP directly | `GrapheneOS/platform_libcore` |
| `libnativehelper` | JNI/native helper boundary | Current `lineage-23.2` manifest uses AOSP directly | Current GrapheneOS 17 manifest uses AOSP directly |
| `hardware/interfaces` | HAL contracts for sensors, graphics, audio, camera, power and other hardware | `LineageOS/android_hardware_interfaces` | `GrapheneOS/platform_hardware_interfaces` |
| `system/sepolicy` | SELinux policy for services, filesystems, device nodes and permissions | `LineageOS/android_system_sepolicy` | `GrapheneOS/platform_system_sepolicy` |

## Kernel and device layer

The table above is not enough to boot a physical phone.

For every target device also preserve and map:

- the exact kernel tree and kernel configuration;
- `device/<vendor>/<device>`;
- `vendor/<vendor>/...` material that can legally be mirrored;
- hardware-specific HALs and sepolicy;
- bootloader and partition-layout assumptions;
- firmware and proprietary blobs as external inputs when they cannot be redistributed.

For the cheap Android Go phone, record the actual ARMv7/Thumb-2 CPU, kernel branch/config, eMMC/UFS/NAND/storage controller details, RAM/zram configuration, device tree, boot image layout and partition table separately from generic AOSP facts.

## Work priorities

### Storage path

For append-only FAT and SD-card work, trace:

`kernel filesystem/block layer -> fs_mgr -> vold -> framework storage APIs -> app/adapter behavior`

Do not claim that a userspace mount or emulator test proves kernel integration or physical removable-media behavior.

### Low-memory path

For Android Go and cheap-device memory work, trace:

`kernel VM + PSI + zram -> lmkd -> libprocessgroup/cgroups -> ActivityManager -> process/application behavior`

Keep kernel pressure, compressed swap, userspace kill policy and framework process policy as separate layers.

### Direct DEX/native path

For Idriç and direct DEX/JNI work, trace:

`DEX -> ART -> libcore -> JNI/libnativehelper -> bionic -> Binder/native services -> HAL/kernel`

Physical ARMv7 evidence remains distinct from emulator or generic AOSP evidence.

### Graphics path

For direct rendering work, trace:

`app/native producer -> Binder/BufferQueue -> SurfaceFlinger -> composer HAL -> kernel/display driver -> panel`

## Mirror policy

Mirror breadth is a goal in its own right. Preserve every verified source family that provides a viable or historically informative answer, then preserve the component repositories selected by those manifests.

The components above are only a first-wave comparison table. They are not the boundary of the OS and they are not a ranking of downstreams.

Keep source-family provenance explicit. AOSP, CyanogenMod, LineageOS, GrapheneOS, CalyxOS, crDroid, BlissOS, Android-x86, /e/OS, DivestOS, and later additions should remain distinguishable even when they share most Git objects.

Cheap-phone changes remain another separately attributable layer. Physical-device observations remain separate from emulator and generic-platform evidence.


## MIRO A1 kernel foundation

Physical receipt on 2026-09-26 identifies the target as `sp9863a` / `s9863a1h10_go_32b` with a vendor kernel reporting `5.15.149-android13-8-g8407b75767d0-dirty`.

The first clean kernel donor is the official Linux 5.15 stable line, pinned initially to `v5.15.149`. Keep the Android common 5.15 line and the eventual MIRO vendor kernel as comparison/provenance sources rather than silently mixing them.

The initial hardware-support set is:

`UART -> I2C -> SPI -> GPIO/EIC -> DMA -> clocks -> regulator/reset/pinctrl infrastructure -> eMMC/SD/SDIO -> USB`

Mainline Linux already contains Spreadtrum/UNISOC implementations for UART, I2C, SPI, GPIO/EIC, DMA, SC9863A clocks and the Spreadtrum SDHCI controller. See `kernel/README.md`, `kernel/SOURCES.tsv`, `kernel/miro-a1.config`, `hardware/miro-a1/README.md`, and `hardware/sc9863a/README.md`.

Do not substitute nearby SoC support for missing board-specific evidence. In particular, the upstream 1H10 device tree is a reference, not proof of MIRO board wiring, and an SC9860-specific pinctrl implementation is not an SC9863A pin map.
