# cheap-phone-os

A working root for a cheap-phone Android operating system based on the Android Open Source Project.

## Upstream

Canonical source:

- manifest: `https://android.googlesource.com/platform/manifest`
- tracking branch: `android-latest-release`
- current platform branch: `android17-release`

GitHub mirror branches in this repository:

- `upstream-manifest` — exact imported history of Google's `android-latest-release` manifest branch
- `cheap-phone-manifest` — the same manifest with the AOSP component remote made absolute so this GitHub repository can be used directly as a Repo manifest source

AOSP is a collection of Git repositories rather than one repository. This repository is the controlling root for the tree; the manifest remains the authoritative inventory of upstream component repositories.

See [AOSP-INTAKE.md](AOSP-INTAKE.md) for the complete acquisition scope, subsystem checklist, single-human product boundaries, device-specific inputs, and acceptance gates. Source preservation and the software installed on the phone are separate decisions; current upstream tracking is not a claim of MIRO hardware compatibility.

See [SOURCES.md](SOURCES.md) for the high-value Android layers and [MIRROR.md](MIRROR.md) for the breadth-first mirror policy. [MIRRORS.tsv](MIRRORS.tsv) is the seed inventory of source families whose branch and tag histories should be preserved here.

## Get the complete upstream tree

```sh
sh _/sync-upstream
```

The script initializes from this repository's `cheap-phone-manifest` branch and synchronizes every AOSP component selected by that manifest from Google's canonical Git service.

## Mirror source families

To import every branch and tag from the currently inventoried source-family repositories into namespaced refs in this repository:

```sh
sh _/mirror-sources
```

The default target is this checkout's push URL for `origin`. Set `MIRROR_TARGET_URL` to another writable Git remote when testing.

## Mirror policy

Do not treat the manifest repository alone as the operating system, and do not treat one downstream as the single answer. Preserve as many technically viable public implementations as practical, including historically useful implementations that answer a distinct design or device-support question. Keep repository identity, history, licenses, provenance, and physical-device evidence boundaries explicit. Cheap-phone-specific work belongs on explicit branches or overlays so source families can continue to be reconciled cleanly.
