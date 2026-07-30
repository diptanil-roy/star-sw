# SL16d_embed 64-bit: P16id compatibility changes

This note summarizes the numerical-compatibility changes used to make the
64-bit `SL16d_embed` reconstruction agree as closely as possible with the
historical 32-bit P16id output. No tracking cuts or physics thresholds were
changed.

## Changes

### Preserve TPC drift-velocity precision

The 32-bit x87 ABI retained extra precision while evaluating
`1e6*mDriftVel`. On x86-64, returning this expression through the historical
`Float_t DriftVelocity()` interface rounded it before downstream code promoted
it to `Double_t`.

- Added `StTpcDb::DriftVelocityDouble()` while retaining the original
  `Float_t` interface for ABI compatibility.
- Changed the four drift-velocity uses in
  `StTpcCoordinateTransform.cc` to call the double-valued accessor.
- Built `StDbUtilities` with `-mfpmath=387 -mpc80`.

### Reproduce historical Sti floating-point behavior

The 32-bit Sti package used x87 arithmetic, whereas the normal x86-64 build
uses SSE arithmetic. Borderline fits can consequently select different hits
or tracks.

- Built the x86-64 `Sti` package with `-mfpmath=387`.
- Kept only `StiTrackNodeHelper::propagateMtx()` on
  `sse2,fpmath=sse` using a narrow GCC function-region pragma. This mixed mode
  matched the reference better than either all-SSE or all-x87 Sti.

The relevant commits are:

```text
d8f0547a7 Preserve P16id TPC drift precision in 64-bit build
4fae054f6 Match historical Sti floating-point behavior on 64-bit
```

## Validation

- The full 64-bit tree builds successfully.
- A second whole-tree `cons` is a true no-op:
  `cons: "." is up-to-date.`
- The first 104 DAQ events match the 32-bit reference in all compared
  event-level quantities: run/event ID, used and good TPC hits, global-track
  count, BTOF count, primary-vertex count, and BEMC/EEMC summaries.
- A stricter MuDst structural comparison finds residual differences in 3 of
  those 104 events. These are discrete track-structure differences, probably
  triggered by remaining floating-point boundary effects, rather than merely
  different serialized float bits.
- In the full sample, the accepted mixed Sti mode has 404 structural mismatch
  events, compared with 439 for ordinary SSE and 980 for package-wide x87.
- The 10-event FZD test completes and produces a valid 10-entry MuDst.

The accepted compiled release is:

```text
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/SL16d_embed_64b
```

## Full remote paths

This README in the accepted compiled source tree:

```text
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/SL16d_embed_64b/README_P16id_COMPATIBILITY.md
```

Files changed specifically for P16id numerical compatibility:

```text
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/SL16d_embed_64b/mgr/Conscript-standard
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/SL16d_embed_64b/StRoot/StTpcDb/StTpcDb.h
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/SL16d_embed_64b/StRoot/StTpcDb/StTpcDb.cxx
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/SL16d_embed_64b/StRoot/StDbUtilities/StTpcCoordinateTransform.cc
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/SL16d_embed_64b/StRoot/Sti/StiTrackNodeHelper.cxx
```

The Git worktree containing the committed versions is:

```text
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/star-sw-git-SL16d_embed_64b
```

The corresponding Git-worktree files have the same relative paths under that
directory, and the README is:

```text
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/star-sw-git-SL16d_embed_64b/README_P16id_COMPATIBILITY.md
```

Detailed validation artifacts are under:

```text
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/diagnostics_20260727
```
