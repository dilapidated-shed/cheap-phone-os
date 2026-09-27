# Repository policy

## Upstream boundary

- Canonical upstream is Google's AOSP Git service, not an archived GitHub mirror.
- Track `android-latest-release`; record the resolved platform branch and manifest head when refreshing upstream.
- AOSP is a manifest-selected collection of repositories. Never describe the manifest repository alone as a complete Android source mirror.
- Preserve upstream repository names, history, licenses, and provenance when importing or reconciling source.

## Source plurality

- AOSP is the provenance baseline, not the only implementation worth preserving.
- Mirror as many technically viable public Android implementations as practical. Include current projects and historically useful projects when they contain a distinct implementation, device-support strategy, hardening approach, or compatibility answer.
- Do not select one downstream as the repository's winner and discard the others.
- Keep each imported repository and source family identifiable in namespaced refs. Identical Git objects may share storage, but provenance mappings must remain explicit.
- Do not erase an existing mirror ref merely because an upstream repository disappears, drops a branch, or rewrites history. Preserve the old state before accepting a non-fast-forward replacement.
- A manifest that points directly at AOSP and a manifest that points at a downstream fork are materially different provenance claims even when most of the tree is shared.

## Cheap-phone work

- Keep cheap-phone-specific changes distinguishable from upstream source.
- Prefer new work on branches and conservative merges.
- Low-memory behavior is an explicit target; do not silently generalize device-specific observations into platform-wide claims.
- Evidence from emulators, generic AOSP targets, and physical phones must be labeled separately.
- The active MIRO A1 source/build tree is the demonstrated dependency closure for that phone, not the whole AOSP manifest. Keep broad source/history mirroring as a separate archival concern; do not build unrelated architectures or hardware merely for completeness.

## Layout

- Build, fetch, and generated work belongs under `_/`.
- `_/aosp/` is a local complete AOSP checkout and is intentionally ignored by the controlling Git repository.
