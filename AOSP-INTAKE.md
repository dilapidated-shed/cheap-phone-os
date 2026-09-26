# AOSP intake for the cheap-phone operating system

Prepared 2026-09-26.

This is an acquisition checklist and subsystem map, not a claim that the source has already been imported, a minimal build manifest, or a flashable MIRO image.

**Acquire broadly; select the installed system separately.** Preserve the complete manifest-selected AOSP source corpus and its provenance. Keeping an implementation for reference does not commit our operating system to its interface, policy, language, database, or resident services.

## 1. What makes the list complete

AOSP is a collection of repositories. The authoritative project-by-project inventory is the canonical manifest, including any included manifests, inherited remotes/revisions, groups, and project-specific overrides. A hand-written list of major directories cannot replace that inventory. See [AOSP source acquisition][download] and [the canonical manifest][manifest].

The repository already has `upstream-manifest` and `cheap-phone-manifest` branches. At inspection, `upstream-manifest` was `ad156f32caaa06dae91c02d443f6a8fe210eaa54`, the previously recorded snapshot, and `cheap-phone-manifest` was `b9cd194a40a8ab0b0a59df085d0b764c8c6f09e6`. These are manifest commits, NOT proof that the component repositories have been mirrored. See [UPSTREAM.md](UPSTREAM.md), [MIRROR.md](MIRROR.md), and [SOURCES.md](SOURCES.md).

### Source acquisition checklist

- [ ] Preserve the canonical `platform/manifest` history and the exact manifest revision used for each acquisition.
- [ ] Acquire every project selected by the intended archival scope, including nested projects and projects not installed on a handset. Record group exclusions explicitly; a normal host-specific/default-group checkout is not automatically an archive of every manifest entry.
- [ ] Preserve project name, checkout path, canonical remote, revision, groups, and provenance. Preserve manifest copy/link rules; do not flatten nested repositories into misleading directory copies.
- [ ] Preserve branch/tag history in the namespaced mirror arrangement described in `MIRROR.md`. A partial clone, shallow project, or current-branch checkout is not a complete historical mirror.
- [ ] Record immutable component commit IDs after synchronization. A manifest whose projects name moving branches is not itself a component revision lock. Capture a resolved manifest, for example with `repo manifest -r`, for the exact synchronized scope.
- [ ] Preserve all manifest-selected `external/` projects and `prebuilts/` inputs, not just libraries whose names appear below. Account for transitive build dependencies before making a smaller product checkout.
- [ ] Preserve license files, notices, component metadata, source attribution, and modifications. Track redistribution constraints separately for non-AOSP vendor inputs.
- [ ] Preserve source and binary input identity separately: a checked-in prebuilt is not proof that its complete corresponding source is present.
- [ ] Preserve build definitions, API/ABI definitions, resource data, tests, tooling, and relevant documentation alongside implementations.
- [ ] Inventory separately selected Android kernel sources and their build manifests. Platform kernel prebuilts and kernel configuration repositories are not the target phone's complete kernel source.

The exhaustive top-level acquisition scope is **all entries in that manifest**, including the families `build/`, `art/`, `bionic/`, `bootable/`, `cts/`, `dalvik/`, `developers/`, `development/`, `device/`, `external/`, `frameworks/`, `hardware/`, `kernel/`, `libcore/`, `libnativehelper/`, `packages/`, `pdk/`, `platform_testing/`, `prebuilts/`, `sdk/`, `system/`, `test/`, `tools/`, and any additional families selected by the actual manifest. These are inventory families, not shell globs to use instead of Repo.

A project's inclusion in the archive must not imply inclusion in the phone image. Conversely, absence from the explanatory tables below does not authorize omitting a manifest project from the archive.

## 2. Keep the upstream track separate from the device track

The canonical `android-latest-release` manifest was checked on 2026-09-26 and selects `android17-release`. That remains the repository's upstream tracking policy, not an automatic decision to install Android 17 on a MIRO A1.

The user's MIRO A1 hardware receipt reports Android 14/API 34 and `armeabi-v7a` application support. Those are device observations, not properties of all AOSP builds. Do not infer physical CPU capabilities, boot-image compatibility, kernel configuration, or supported HAL versions from the application ABI alone.

Maintain two distinguishable records:

| Record | What it is for | What must be pinned |
| --- | --- | --- |
| Current upstream corpus | Source preservation, comparisons, future implementation work | Canonical manifest commit, resolved component commits, acquisition groups and source history coverage |
| MIRO-compatible baseline | Bring-up and regression comparisons against the physical device | Chosen framework release, exact vendor/ODM inputs, kernel/configuration/modules, VINTF requirements, ABI, partition layout and firmware receipts |

Do not mix arbitrary Android 14, Android 17, and vendor binaries merely because their directory names match. HAL contracts, vendor-library interfaces, linkage, kernel requirements, and boot configuration have to agree. [VINTF][vintf] records what the framework and device provide and require.

Paths below are a map of the observed current corpus, with older-path notes where important. Resolve every actual acquisition through its own release manifest. A path appearing in Android 17 does not establish that it exists or works in Android 14.

## 3. Build and source infrastructure

For every row, acquire the implementation, build files, public and private interfaces needed by the selected build, tests, data files, and dependencies. The disposition column describes the phone product, not archival permission to delete source.

| ID | What to acquire | Source locations / scope | Product disposition |
| --- | --- | --- | --- |
| B01 | Product definitions, Android Go/low-RAM configuration, image composition, resource overlays, release flags | `build/make`, `build/release`, device/product configuration selected by the manifest | Retain the reference build; make our product choices in separate overlays or branches |
| B02 | Build graph and build generators | `build/soong`, `build/blueprint`, other manifest-selected build components | Host infrastructure; do not install the build system on the phone |
| B03 | Reference toolchains and all compiler/runtime/build dependencies | Manifest-selected `prebuilts/`, compiler sources and build dependencies under `external/`; native, DEX, Java/Kotlin, Rust, Go and code-generation inputs required by the chosen reference build | Preserve reproducibility; this does not choose languages for our replacement implementations or require compilers on the handset |
| B04 | APK/resources/DEX production, signing and verification | Resource tools in `frameworks/base`, build tooling, SDK/API data, DEX toolchain and signature tooling selected by the manifest | Required for compatible artifacts; independent of whether our source is Java, Idriç or another language |
| B05 | Boot, ramdisk, sparse-image, filesystem and partition-image production | `system/tools/mkbootimg`, `system/core/mkbootfs`, `system/core/libsparse`, `system/fs/fs_mgr`, `bootable/recovery`, relevant `external/` utilities | Retain image-format and recovery knowledge; select actual formats from the device receipt |
| B06 | Complete dependency closure and build receipts | Resolved manifest, generated build dependency information, installed-file inventory, symbols, image metadata and hashes | Required before calling a reduced source selection complete |

Changing the source language is a separate compiler/translation project. Removing the reference Java compiler, Rust compiler, framework libraries or runtime from an otherwise unchanged AOSP build does not implement their replacements.

## 4. Kernel-facing userspace, runtime and process machinery

[Android's architecture][architecture] separates kernel, HAL, native services, runtime, framework and apps. Preserve those boundaries in the inventory even where our eventual implementation combines or replaces pieces.

| ID | What to acquire | Source locations / scope | Product disposition |
| --- | --- | --- | --- |
| R01 | Android kernel integration: Binder, process credentials, namespaces, syscall filtering, cgroups, scheduling, timers, memory pressure, swap/zram, storage, device drivers | Separately selected Android kernel source/build manifest, `kernel/configs`, kernel tests and relevant userspace interface code | Required kernel functions; the physical vendor kernel is a separate input |
| R02 | PID 1, service startup/restart, device-node handling, properties, early mounts and boot ordering | `system/core` including init, rootdir and property machinery; current split repositories such as `system/libueventd-rs` where the selected release uses them | Retain for bring-up; simplify only with explicit service/device dependencies |
| R03 | Native ABI, C library, dynamic linker, allocator and native support libraries | `bionic`, libc++ and other selected native runtime inputs, `system/libbase`, utilities in `system/core`, `system/libziparchive`, `system/unwinding`, other linked libraries | Keep vendor/native ABI compatibility until an actual replacement is demonstrated |
| R04 | Binder IPC and service discovery | Binder and service-manager code in `frameworks/native`; `system/hwservicemanager`, `system/libhidl`, `system/libhwbinder`, `system/libfmq`, `system/tools/aidl`, `system/tools/hidl` as required | Preserve existing vendor contracts; do not replace every IPC protocol at once |
| R05 | HAL contracts, versioning and vendor compatibility checks | `hardware/interfaces`, `hardware/libhardware`, `hardware/libhardware_legacy`, `frameworks/hardware/interfaces`, `system/hardware/interfaces`, `system/libvintf`, selected vendor-interface snapshots | Required interface knowledge; reference HALs are not MIRO drivers |
| R06 | ART/DEX execution, verification, compilation, garbage collection, boot/runtime images and JNI | `art`, `libcore`, `libnativehelper`, selected ART module/prebuilt/build inputs | Retain for Android apps and current direct-DEX work; ART alone is not the Android application framework |
| R07 | Zygote, application startup, component lifecycle and system services | `frameworks/base`, current `system/zygote` split where selected, supporting framework libraries | Compatibility dependency; replace behavior deliberately rather than deleting by name |
| R08 | Package installation, signatures, resources, intents, providers, notifications, alarms and jobs | `frameworks/base`, `packages/modules/Permission`, `packages/modules/IntentResolver`, supporting modules and API surfaces | Retain contracts needed by installed apps; connect our CLI/UI and durable job design separately |
| R09 | APEX loading, linker namespaces, classpaths, module metadata and API-extension compatibility | `system/apex`, `system/linkerconfig`, ART/framework module definitions, `packages/modules/common`, `packages/modules/ModuleMetadata`, `packages/modules/SdkExtensions` and release-selected supporting modules | Required while the retained system uses these packaging/runtime boundaries |

The reference [system/core tree][core-tree] and [ART tree][art-tree] contain more than the named entry points. Acquire their complete project contents and dependencies.

## 5. Low-memory operation, power and physical interaction

These are not optional finishing touches for a cheap phone.

| ID | What to acquire | Source locations / scope | Product disposition |
| --- | --- | --- | --- |
| L01 | Memory pressure handling, process kill policy and measurement | `system/memory/lmkd`, `system/memory/libmeminfo`, other selected memory libraries, framework process policy | Retain and measure on each physical target |
| L02 | Compressed memory/swap coordination and newer memory-management designs | Kernel VM/zram code; current `system/memory/mmd` and `system/memory/guardian` as upstream reference where selected | Do not assume newer daemons run on the A1 kernel; compare and port deliberately |
| L03 | Process groups, task profiles, priorities and resource accounting | `system/core/libprocessgroup`, task/cgroup configuration, kernel support and framework scheduler interactions | Keep resource control separate from human accounts and job identity |
| L04 | Suspend/resume, wake locks, alarms, battery/charging, thermal limits and power hints | `system/core` health/suspend code, framework power/alarm/thermal services, `system/hardware/interfaces` and `hardware/interfaces` power/health/thermal contracts | Essential; retain vendor charging and thermal protections during bring-up |
| L05 | Low-RAM product behavior and process-death recovery | Build/product settings, framework memory policy, application save/restore contracts | Measure actual memory and restart behavior; no unmeasured promised RAM saving |
| D01 | Display composition, buffers, synchronization and GPU API integration | `frameworks/native`: SurfaceFlinger, BufferQueue, graphics/input libraries; composer/allocator/mapper HALs; selected EGL/GLES/Vulkan support | Keep the working display/GPU path; the vendor GPU driver remains separate |
| D02 | Raster graphics, text layout, Unicode, scripts and fonts | `external/skia`, `frameworks/minikin`, `external/icu`, `external/harfbuzz_ng`, `external/freetype`, font projects, locale/timezone data | Needed for readable UI and multilingual reading; select installed fonts separately from archival scope |
| D03 | Touch, hardware keys, keyboard, selection and clipboard | Native input services, framework input and IME contracts, device keylayout/calibration data, `packages/inputmethods/LatinIME` as a donor | Keep input functionality; substitute our keyboard/UI independently |
| D04 | Accessibility, magnification, navigation assistance, captions and speech interfaces | Framework accessibility/text-to-speech interfaces and release-selected implementations | Preserve interface requirements; a complete speech engine or voices may require separate non-AOSP inputs |
| D05 | Sensors, rotation, vibration, lights, flash and location/GNSS | Native/framework services, `hardware/interfaces`, relevant system interfaces and device implementations | Enable actual hardware only; GNSS, geocoding and network location are distinct services |
| D06 | USB roles, accessories, input devices and file transfer | `system/usb`, USB support in core/frameworks, USB HALs, `packages/services/Mtp`, device configuration | Retain the supported physical connection/recovery path; do not infer ports/features from generic AOSP |

See [low-memory killer design][lmkd], [graphics composition][graphics], and [HAL architecture][hal]. These specify useful contracts; none is a physical MIRO acceptance result.

## 6. Calls, messages and networking

| ID | What to acquire | Source locations / scope | Product disposition |
| --- | --- | --- | --- |
| N01 | IP networking, routing, DNS, connection selection, DHCP, validation and captive portals | `system/netd`, `packages/modules/Connectivity`, `packages/modules/DnsResolver`, `packages/modules/NetworkStack`, `packages/modules/CaptivePortalLogin`, selected BPF/firewall/network utilities | Keep functional network plumbing; choose our own trust/connection policy |
| N02 | Wi-Fi client, authentication, hotspot and tethering | `packages/modules/Wifi`, `system/connectivity/wificond`, release-selected supplicant/AP implementation, connectivity/tethering code and Wi-Fi HALs | Required for Wi-Fi; actual driver and firmware are vendor inputs |
| N03 | Bluetooth transport and profiles | `packages/modules/Bluetooth`, Bluetooth HALs and device configuration | Retain the profiles actually used, including call audio; chipset transport/firmware are separate |
| N04 | Cellular registration, SIM/subscriptions, radio control, mobile data, SMS and carrier configuration | `hardware/ril`, radio HALs in `hardware/interfaces`, `frameworks/opt/telephony`, `packages/services/Telephony`, current `packages/modules/Telephony` where selected, `packages/apps/CarrierConfig`, telephony provider | Essential phone functionality; preserve vendor and carrier-specific integration |
| N05 | Call routing, in-call services, IMS/VoLTE and Wi-Fi calling integration | `packages/services/Telecomm`, current `packages/modules/Telecom`, `frameworks/opt/net/ims`, release-selected `ImsMedia`/`ImsStack`, entitlement, Iwlan and qualified-network components | Keep only the branch/carrier combination actually supported; importing a newer generic IMS component does not replace vendor IMS automatically |
| N06 | MMS, emergency information and cell broadcasts | `packages/services/Mms`, `packages/modules/CellBroadcastService`, `packages/apps/CellBroadcastReceiver`, `packages/apps/EmergencyInfo`, telephony configuration and APIs | Treat as a separate acceptance area, not just whether the dialer launches |
| N07 | TLS, certificates, crypto interfaces and protected key use | `external/boringssl`, `external/conscrypt`, certificate-store projects selected by the release, `packages/apps/KeyChain`, `packages/apps/CertInstaller`, keystore interfaces | Keep maintained authentication/encryption; choose managed trust changes explicitly |
| N08 | VPN/IPsec and optional network capabilities | `packages/modules/IPsec`, framework VPN contracts, release-selected connectivity components | Product-selectable; retain required contracts for apps that use them |

[AOSP's IMS documentation][ims] distinguishes framework integration from the carrier/device IMS implementation. A running application process or a data connection is not proof that voice calls, SMS, or emergency behavior work.

## 7. Audio, camera, files and persistent state

| ID | What to acquire | Source locations / scope | Product disposition |
| --- | --- | --- | --- |
| S01 | Audio recording/playback, routing, call audio and policy | `frameworks/av`, `system/media`, native/framework audio interfaces and audio HAL contracts | Essential; vendor mixer paths, tuning and firmware are separate |
| S02 | Camera capture, camera service/provider contracts, image processing and metadata | `frameworks/av`, camera HALs/interfaces, selected image libraries and camera app donors | Preserve capture before replacing UI; vendor camera binaries/calibration are separate |
| S03 | Media extraction, parsing, codecs and hardware acceleration | `frameworks/av`, `packages/modules/Media`, selected codec/image/audio/video libraries under `external/`, vendor codec interfaces | Keep required formats and parser isolation; hardware codecs and DRM services may require external inputs |
| S04 | Mounts, volumes, removable SD, storage events, shared-file access and USB media access | `system/fs/fs_mgr`, `system/vold`, storage code in `frameworks/base`, `packages/providers/MediaProvider`, relevant FUSE and MTP code | Main replacement boundary for filesystem-first storage. On older branches, notably the existing Android 14 map, fs_mgr is within `system/core/fs_mgr` |
| S05 | Filesystem creation/check/repair and image support | Manifest-selected `external/e2fsprogs`, `external/f2fs-tools`, `external/exfatprogs`, FAT/EROFS/verity utilities, compression/archive libraries; matching kernel filesystems | Select actual device formats. Userspace tools alone cannot change kernel filesystem semantics |
| S06 | File/metadata encryption, keys, credential verification and secure-hardware interfaces | `system/vold`, `system/security`, `system/keymaster`, `system/keymint`, `system/gatekeeper`, release-selected Weaver/secure-storage components and HALs | Keep protection against lost/stolen devices; vendor trusted execution support must be inventoried separately |
| S07 | Contacts, calendar, call log, blocked numbers, messaging and download/provider API compatibility | Relevant projects under `packages/providers/`, framework provider contracts and existing persistence dependencies including `external/sqlite` | Compatibility donor, not the storage architecture for our new OS. Replace selected providers behind stable interfaces before removing their dependencies |
| S08 | Time, timezone, locale, MIME types, certificates and other runtime data | `system/timezone`, ICU/runtime internationalization, media/MIME data, font and certificate projects, configuration overlays | Account for required data as well as executable code |

The [media tree][av-tree] and [storage documentation][storage] identify the existing divisions. Our own durable state should use ordinary files/directories, atomic replacement, append-only records and explicit recovery where appropriate. Keeping SQLite source for an unchanged Android provider is not permission to introduce a new database-backed source of truth.

## 8. Protection, updates, UI and ordinary applications

| ID | What to acquire | Source locations / scope | Product disposition |
| --- | --- | --- | --- |
| P01 | Existing isolation mechanisms and the policy they currently enforce | Kernel security mechanisms, `external/minijail`, `external/selinux`, `system/sepolicy`, platform/vendor policy and service access checks | Preserve as baseline/reference. SELinux is not the intended final policy model; its removal requires replacement enforcement and dependency work, not just permissive mode |
| P02 | Permission prompts, app operations, sensor privacy and delegation interfaces | `packages/modules/Permission`, framework permissions/AppOps/privacy and IPC checks | Retain protection requirements; redesign UI and authority model for one human and many program/job principals |
| P03 | Verified boot, image authentication and rollback handling | `external/avb`, boot/recovery integration, chosen bootloader interfaces and signing tools | Preserve update integrity and recovery. Owner-controlled keys depend on actual bootloader support; do not assume the A1 can be unlocked or relocked safely |
| P04 | OS/module updates, rollback, recovery and factory reset | `system/update_engine`, `bootable/recovery`, `system/apex`, release-selected crash/rollback components, build release tooling and boot-control HALs | Select A/B, virtual A/B or other layout from the actual device; do not assume a partition scheme |
| P05 | Initial setup, external-account authentication and local credential setup | Framework account/credential contracts, `packages/apps/Provision`, setup/settings components; managed provisioning as a reference only | One human does not mean one email/service account. No Google account or enterprise enrollment requirement in our design |
| U01 | System chrome: home, lockscreen, status, quick controls, navigation, notifications and settings | `frameworks/base` SystemUI/windowing, `packages/apps/Settings`, `packages/apps/Launcher3`, `packages/apps/SystemUIGo` as applicable | Replacement candidates; retain the underlying necessary functions |
| U02 | Everyday application donors | `packages/apps/Dialer`, `Contacts`, `Messaging`, `Calendar`, `DeskClock`, `Camera2`, gallery/media apps and their supporting projects | Decide individual retention/replacement through the product manifest rather than automatically shipping every donor |
| U03 | File picker, downloads, sharing, package install and useful peripheral services | `packages/apps/DocumentsUI`, download/provider code, intent resolver, installer/signature machinery, MTP and optional print services | Preserve interoperable actions while replacing presentation and storage policy |
| U04 | Web rendering and Android WebView integration | `external/chromium-webview`, framework WebView integration, current `packages/modules/WebViewBootstrap` and other branch-selected packaging | A working browser integration needs a maintained engine, its source/prebuilt provenance and updates. An AOSP WebView prebuilt repository is not the entire Chromium source tree |
| U05 | Configuration/module dependencies and optional framework services | Every remaining manifest-selected module, its API consumers and build/runtime dependencies | Inventory all; enable only selected product functions. Do not remove a module merely because its name sounds optional |

[APEX][apex] is a packaging/runtime dependency, not a reason to adopt Google's service policies. [SELinux documentation][selinux] describes the protections of the existing Android baseline; the proposed replacement model must state what enforces equivalent required boundaries.

## 9. Diagnostics and acceptance material

| ID | What to acquire | Source locations / scope | Product disposition |
| --- | --- | --- | --- |
| T01 | Local logs, crash dumps, stack unwinding, boot diagnostics and tracing | `system/logging`, core debugger/boot/watchdog code, `system/unwinding`, `system/extras`, manifest-selected Perfetto/profiling tools | Retain bounded, permission-controlled diagnostics; local diagnostics do not imply remote telemetry |
| T02 | Debug/recovery communications | `packages/modules/adb`, `system/core/fastboot`, recovery utilities and relevant host/device support | Development/recovery functions, not an unauthenticated production service or the mandatory everyday interface |
| T03 | Unit, integration, platform and vendor-interface tests | Tests in every project, `cts`, `test/vts`, `test/vts-testcase/hal`, `test/vts-testcase/kernel`, `platform_testing`, Tradefed/harness and related manifest entries | Keep useful tests even where the custom OS intentionally diverges from Android certification requirements |
| T04 | Emulator/reference target definitions | Selected `device/generic/` and `device/google/cuttlefish` projects, emulator/kernel/prebuilt inputs | Useful baseline tests; label results as emulator/reference, never physical MIRO acceptance |
| T05 | Device acceptance checklist and reproducible receipts | Our own device-specific tests plus applicable AOSP tests | Separate A1/C67 receipts; verify ABI, boot, graphics, input, suspend, charging, radios, calls/messages, storage, updates and isolation independently |

## 10. Sources to preserve without adopting their product model

The archive remains broad. The intended phone image does not include a feature solely because it is in AOSP.

| Existing feature/source | Our product decision |
| --- | --- |
| Multiple human users, guests, work profiles, private-space/profile switching; associated framework code and `packages/apps/Multiuser` | No multi-human account model. Adult and child configurations are per-device policies, not separate Unix/Android people sharing one handset. Existing single-user compatibility APIs may need to remain while Android components still call them |
| SELinux policy and tooling | Keep reference/source provenance, but design a different enforcement model for the replacement OS. An unchanged stock-compatible baseline and the replacement OS are distinct artifacts |
| Enterprise managed provisioning and device-owner management | Donor/reference only; not the structure of adult/child policy in our OS |
| AdServices, OnDevicePersonalization and app-prediction/personalization implementations | Not part of the requested baseline product; preserve source while checking dependency effects of exclusion |
| DeviceLock and other remotely imposed device-management features | Not implied by child policy or cheap hardware; excluded unless a separately agreed function requires them |
| TV, automotive, desktop, screensaver, wallpaper and other non-target products | Preserve source families; omit unrelated product payloads and resident services from the phone |
| NFC, UWB, secure element, context hub, Thread, neural accelerators, virtualization and other optional hardware/services | Enable only for demonstrated hardware or a specific approved use; no inference from AOSP's inclusion |
| SQLite-backed providers and Java/Kotlin/Rust implementations | Preserve working donors and reference-build dependencies. New storage and source-language decisions are separate and must follow the user's design constraints |
| AOSP test keys and debug service configurations | Test material only; not the keys or exposure policy for a deployed phone |

**One human does not mean every process is root, every app shares a UID, or hardware access is unrestricted.** Preserve process/job identity and isolation while removing the multi-human account model. Child policy needs a protected guardian-management authority; that is not a second daily login account.

## 11. Required inputs that do not come from generic AOSP

AOSP's [download documentation][download] explicitly separates generic source from device-specific binaries. For each physical model, create an independent inventory; sharing a brand does not establish shared drivers or firmware.

- [ ] Exact model/board identifiers, application and kernel architectures, build fingerprint, release/security level and observed hardware capabilities.
- [ ] Exact vendor kernel source and patches where available; configuration, device-tree sources/DTB/DTBO, module binaries and source, compiler/build parameters and interface dependencies.
- [ ] Bootloader/unlock/verified-boot behavior and a demonstrated recovery route. Do not assume unlockability from the presence of an OEM-unlocking menu or generic fastboot support.
- [ ] Partition table and image formats: whichever of boot, init_boot, vendor_boot, recovery, vbmeta, super, system, vendor, odm, product, system_ext, metadata and userdata actually exist. Record A/B state if applicable rather than manufacturing nonexistent partitions.
- [ ] Device/product definitions: BoardConfig, product makefiles, overlays, fstab, init configuration, device-node rules, VINTF manifests/matrices, feature and permission XML, and vendor-library linkage requirements.
- [ ] Modem/baseband firmware, vendor radio implementation, IMS/VoLTE/Wi-Fi-calling services and carrier configuration required by the target SIM/network.
- [ ] GPU/display binaries, HALs and matching kernel support. A PowerVR observation on one device must not be generalized to another model.
- [ ] Camera HAL/provider binaries, sensor support, image-processing firmware, tuning and calibration.
- [ ] Audio HAL, DSP firmware if present, mixer/routing configuration, calibration and call-audio dependencies.
- [ ] Wi-Fi/Bluetooth/GNSS firmware, transport libraries, HALs, calibration and configuration.
- [ ] Touch, sensors, lights, vibrator, USB, battery/charging, power and thermal implementations and configuration actually required by the board.
- [ ] Trusted execution/keystore/keymint/gatekeeper/secure-storage dependencies supported by this hardware, including vendor firmware and interfaces.
- [ ] Recovery/stock firmware provenance and checksums; keep a tested return path before destructive changes.
- [ ] Extraction/reconstruction instructions, hashes, licensing/redistribution status and missing-input status for every required binary.

Do not commit IMEIs, device-unique radio/calibration partitions, personal userdata, credentials, authentication tokens, private signing keys or unredacted diagnostic dumps to the public repository. Preserve required sensitive material privately; use redacted structural receipts and hashes where appropriate.

Google Play services, Play Store, Google's proprietary apps, carrier cloud services, commercial DRM implementations, map tiles, literature, and our selected third-party applications are not obtained merely by acquiring the AOSP platform tree. Inventory required non-AOSP software/content separately rather than silently treating it as a platform dependency.

## 12. Things we must implement rather than fetch from AOSP

These are requirements of this project, not claims about finished components.

- [ ] **Single-human policy:** a per-device adult/child configuration, a separately protected guardian-management path where needed, no account-switching model for routine use.
- [ ] **Program/job authority:** execution identities, explicit delegation and revocation, authority-limited hardware/data access, and enforcement for native processes as well as apps.
- [ ] **Provenance:** authenticated creator/origin/parent information and rules for derived jobs/files. A hostname, filename, signer claim or browser-written label must not itself grant authority.
- [ ] **Filesystem-first state:** ordinary files/directories, atomic updates, append records, recoverable transfers and explicit persistence boundaries. Supporting metadata must survive the operations and filesystems actually used.
- [ ] **Crawl Space primitives:** selected changes to filesystem/storage behavior, append semantics, placement and temporary-data lifetime. Do not mislabel a userspace timer or directory convention as kernel-enforced expiration.
- [ ] **Durable work:** task journals, restart/recovery after application/renderer/host-process death, and local notification/continuation behavior. Do not rely on keeping an Activity visible.
- [ ] **Provisioning and product composition:** Cat Food-driven desired state, our actions/CLI/UI clients, selected everyday applications, and one operating system with different device policies.
- [ ] **Replacement acceptance tests:** successful ordinary functions and unsuccessful forbidden accesses; explicit source/build/ABI/physical-device receipts.

The old Android implementations are donors and test oracles. Their policy is not automatically ours.

## 13. What goes where in this repository

Keep the existing layout and provenance policy rather than copying all AOSP directories into this control branch.

| Material | Destination / representation |
| --- | --- |
| This intake checklist and design decisions | Root documentation, linked from README and AGENTS |
| Canonical manifest history | Existing `upstream-manifest` namespaced source branch |
| Manifest adapted for this checkout | Existing `cheap-phone-manifest` branch, with adaptation clearly recorded |
| Complete source checkout | Existing ignored `_/aosp/` layout; a separate checkout for a different device-compatible release |
| Mirrored component and downstream histories | Namespaced refs under the existing `MIRROR.md` policy; retain source identity |
| Resolved manifests, acquisition receipts, project inventories and build outputs | Under `_/`; deliberately retain selected non-sensitive receipts without committing whole build/source worktrees |
| Our implementation changes | Explicit development branches/overlays, not silently rewritten upstream snapshots |
| Device-specific inputs | Separately identified per-device source/configuration and external/private binary inventory; no guessed MIRO device tree presented as existing |

A plain-text/TSV project inventory should record `checkout_path`, `upstream_project`, `remote`, `manifest_revision`, `resolved_commit`, `groups`, `source_or_prebuilt`, `product_role`, `license_reference`, `acquisition_status`, and `evidence`. This is an audit/export format, not a new OS database.

The existing `_/sync-upstream` script is a source checkout entry point. Its existence is not a receipt that synchronization ran, that every manifest group was included, that all history was mirrored, or that the phone can boot the result.

## 14. Completion gates

- [ ] **Inventory complete:** every project in the chosen manifest scope accounted for, plus separately identified kernel and device inputs. No silent unclassified omissions.
- [ ] **Source acquired:** actual objects/files present, revisions and history coverage recorded, licenses retained, missing/proprietary pieces named.
- [ ] **Reference build reproduced:** concrete target, toolchain, product configuration, resolved inputs and build result recorded.
- [ ] **Reduced product dependency closure demonstrated:** every installed binary/module/resource and build dependency accounted for; nothing removed only by guesswork.
- [ ] **Physical bring-up demonstrated:** boot/recovery, display/touch, power/thermal/suspend, network/radio, calls/messages, audio/camera, sensors and storage tested on the named phone.
- [ ] **Replacement policy demonstrated:** process isolation, file/network/device authority, child-policy protection, provenance handling, update trust and recovery tested, including denial cases.
- [ ] **Daily-use reliability demonstrated:** low-memory/process-loss recovery, incoming calls/messages while idle, alarms, files/forms/photos and interrupted updates/transfers tested.

**Status of this change:** checklist/documentation only. It does not download the full AOSP source, resolve a full build dependency graph, extract vendor binaries, implement replacements, flash a phone, or establish Android 17 compatibility with the MIRO A1. The source acquisition scope is complete by manifest definition; the smallest sufficient bootable MIRO product remains to be established by the build and physical-device gates above.

## Primary references

- [Canonical current AOSP manifest][manifest]
- [AOSP source acquisition and device binaries][download]
- [AOSP architecture][architecture]
- [Hardware abstraction layer][hal]
- [VINTF compatibility requirements][vintf]
- [Generic kernel image and vendor separation][gki]
- [Native core tree][core-tree]
- [ART tree][art-tree]
- [Media/audio/camera tree][av-tree]
- [Low-memory killer daemon][lmkd]
- [SurfaceFlinger and WindowManager][graphics]
- [Storage][storage]
- [IMS integration][ims]
- [APEX][apex]
- [Existing Android SELinux architecture][selinux]
- [Repo manifest format][repo-format]

[manifest]: https://android.googlesource.com/platform/manifest/+/refs/heads/android-latest-release/default.xml
[download]: https://source.android.com/docs/setup/download
[architecture]: https://source.android.com/docs/core/architecture
[hal]: https://source.android.com/docs/core/architecture/hal
[vintf]: https://source.android.com/docs/core/architecture/vintf
[gki]: https://source.android.com/docs/core/architecture/kernel/generic-kernel-image
[core-tree]: https://android.googlesource.com/platform/system/core/+/refs/heads/android17-release/
[art-tree]: https://android.googlesource.com/platform/art/+/refs/heads/android17-release/
[av-tree]: https://android.googlesource.com/platform/frameworks/av/+/refs/heads/android17-release/
[lmkd]: https://source.android.com/docs/core/perf/lmkd
[graphics]: https://source.android.com/docs/core/graphics/surfaceflinger-windowmanager
[storage]: https://source.android.com/docs/core/storage
[ims]: https://source.android.com/docs/core/connect/ims
[apex]: https://source.android.com/docs/core/ota/apex
[selinux]: https://source.android.com/docs/security/features/selinux
[repo-format]: https://gerrit.googlesource.com/git-repo/+/HEAD/docs/manifest-format.md
