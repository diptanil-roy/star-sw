# SL16d_embed 64-bit SL7 port

This checkout contains the source changes used to port the STAR
`SL16d_embed_v3` release to the 64-bit SL7 architecture
`sl73_x8664_gcc485` for use from AlmaLinux 9 submit nodes through the STAR
SL7 Singularity container.

## Repository provenance

- Upstream: `https://github.com/star-bnl/star-sw.git`
- Base tag: `SL16d_embed_v3`
- Base commit: `b4b7b0298b77757b3e3e80f7ccdb88ac81052159`
- Port branch: `SL16d_embed_64b`
- Port commit: `aa8e1e3ce` (`Port SL16d_embed to 64-bit SL7`)
- Local checkout:
  `/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/star-sw-git-SL16d_embed_64b`

The Git tag and the centrally installed release do not contain exactly the
same revisions of every file. The port was therefore transferred as targeted
64-bit changes instead of replacing complete source files wherever possible.

## Summary of changes

### Build system and dependency discovery

- `mgr/ConsDefs.pm`
  - Updated the SL7 64-bit compiler, Fortran, dependency, include-path, and
    linker configuration.
  - Added the generated `agetof.def` path and propagated `CPATH` and
    `PYTHONPATH`.
  - Added FastJet and GSL include package definitions.
  - Updated MySQL, log4cxx, XML, Qt, and STAR-spack discovery used by the
    AlmaLinux 9/SL7 container environment.
- `mgr/Construct`
  - Enabled the GEANT3, GCALOR, `root4star`, StarSim, Qt, and online packages
    for the 64-bit build instead of excluding them as legacy 32-bit targets.
- `mgr/cons`
  - Added modern-Perl handling for relative `Conscript` files (`./...`).
- `asps/rexe/Conscript`
  - Updated the 64-bit executable/link rules used to produce `root4star` and
    the simulation executables.
- `QtRoot/Conscript`
  - Added the QtRoot build/install rule and made `qmake` resolve from the
    configured command environment rather than a user-specific path.

### ROOT 5 dictionary and Jevp cleanup

ROOT 5 `rootcint` could not parse the low-level RTS platform, pthread, and
VxWorks compatibility headers on 64-bit Linux. The Jevp changes keep those
implementation-only DAQ/RTS includes out of public dictionary headers, use
forward declarations where possible, and include the concrete definitions in
the implementation files.

Affected Jevp files:

- `OnlTools/Jevp/StJevpBuilders/JevpBuilder.h`
- `baseBuilder.h`, `bbcBuilder.h`, `bemcBuilder.h`, `daqBuilder.h`
- `eemcBuilder.h`, `fgtBuilder.h`, `fpdBuilder.h`, `fpsBuilder.h`
- `gmtBuilder.h`, `hltBuilder.{h,cxx}`, `hltphiBuilder.{h,cxx}`
- `istBuilder.h`, `l3Builder.h`, `l4Builder.{h,cxx}`
- `mtdBuilder.h`, `ppBuilder.h`, `pxlBuilder.h`, `ssdBuilder.h`
- `tofBuilder.h`, `tpxBuilder.h`, `trgBuilder.h`, `upcBuilder.h`
- `vpdBuilder.h`
- `OnlTools/Jevp/StJevpPlot/JLatex.h`
- `OnlTools/Jevp/StJevpPresenter/JevpGui.cxx`

`JevpGui.cxx` also replaces a pointer-to-`int` conversion with a
pointer-width-safe conversion.

### FMS trigger dictionary separation

- Added `StRoot/StSpinPool/StFmsTriggerMaker/Crate.cxx`.
- Reduced `Crate.hh` to dictionary-safe declarations and forward declarations.
- Moved low-level RTS trigger implementation details into the `.cxx` file.
- Updated `StFmsTriggerMaker.cxx` to use the separated implementation.
- Added `StRoot/RTS/trg/include/trgDataDefs.h` with the trigger data structures
  required by the implementation.

### GEANT3 and StarSim 64-bit fixes

- `StRoot/St_geant_Maker/navigate.g`
  - Uses pointer-width-safe node/address storage.
- `asps/Simulation/starsim/deccc/locf.c`
  - Uses the later pointer-token implementation with a stable nonzero base.
  - Falls back to `csvptokn_`/`csvplong` when an address cannot be represented
    directly instead of aborting on the valid 64-bit token path.
- `asps/Simulation/starsim/atgeant/agsbegm.age`
  - Uses a fixed local RZ buffer rather than deriving a buffer through a
    truncated address calculation.
- `asps/Simulation/starsim/dzdoc/mzpaw.F`
  - Adjusted the PAWC/ZEBRA initialization path for the 64-bit memory layout.

The base tag already contained several later 64-bit fixes that were needed by
the installed release (including pointer-safe `idisp`, `locf`, GEANT bank, and
`St_geant_Maker` changes), so those were retained rather than reapplied as
older complete-file replacements.

### Restored generator IDL inputs

Added the IDL inputs required by the 64-bit build:

- `pams/gen/idl/eg_event.idl`
- `pams/gen/idl/eg_gener.idl`
- `pams/gen/idl/eg_track.idl`
- `pams/gen/idl/eg_vertex.idl`
- `pams/gen/idl/particle.idl`

### Source hygiene

- No `.sl73_x8664_gcc485` objects, libraries, binaries, build logs, temporary
  scripts, backup files, or rejected patch files are committed.
- User-specific `/gpfs01/.../droy1` paths were removed from committed source.
- Imported legacy files had trailing whitespace normalized during staging.

## Build environment

Start the clean SL7 container from an AlmaLinux 9 `starsub` node:

```csh
singularity exec -e \
    -B /direct -B /star -B /afs -B /gpfs -B /sdcc/lustre02 \
    /cvmfs/star.sdcc.bnl.gov/containers/rhic_sl7.sif \
    csh
```

Inside the container:

```csh
starver SL16d_embed
setup 64b

setenv STAR /gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/SL16d_embed_64b
setenv STAR_LIB $STAR/.${STAR_HOST_SYS}/LIB
setenv STAR_BIN $STAR/.${STAR_HOST_SYS}/BIN
setenv QTROOTSYSDIR $STAR/.${STAR_HOST_SYS}
setenv NODEBUG yes

set path = ( \
    $STAR_BIN \
    $STAR/local-qmake \
    /star/nfs4/AFS/star/packages/StAF/SL00a/.i386_redhat61/bin \
    $path \
)

setenv LD_LIBRARY_PATH ${STAR_LIB}:${QTROOTSYSDIR}/lib:${ROOTSYS}/lib:${LD_LIBRARY_PATH}
rehash

cd $STAR
$STAR/mgr/cons
```

The 32-bit `stic` above is only an IDL-to-source generator. Generated sources
are compiled by the 64-bit compiler.

## Validation completed on the compiled release

Compiled release tree:

```text
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/SL16d_embed_64b
```

Validation results:

- Full `cons` completed without fatal build errors.
- A subsequent no-op `cons` returned status `0` and ended with
  `cons: "." is up-to-date.`
- Local `root4star` is a 64-bit executable.
- `runBfc.C` processed all 10 events from
  `rcf22000_15117062_0000_1_10evts.fzd` successfully.
- The produced MuDst contains exactly 10 entries.

No-op build evidence:

```text
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/SL16d_embed_64b/validation_true_noop_20260718.log
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/SL16d_embed_64b/validation_true_noop_20260718.status
```

Runtime validation output:

```text
/gpfs01/star/pwg/droy1/STAR-Workspace/D0Analysis/PythiaScript/validation_runBfc_SL16d_64b_final_20260718_2352/rcf22000_15117062_0000_1_10evts.MuDst.root
```

### P16id real-data reconstruction regression

The local 64-bit release was also tested against the first 10 events of the
2015 P16id reference DAQ file:

```text
/star/rcf/test/daq/2015/148/st_physics_16148020_raw_4000024.daq
```

using the chain recorded in the original P16id production log:

```text
DbV20160710 pp2015c btof mtd mtdCalib pp2pp fmsDat fmsPoint fpsDat BEmcChkStat -evout CorrX OSpaceZ2 OGridLeak3D -hitfilt
```

Results:

- The 10 local run/event IDs exactly match the first 10 events in the original
  P16id production log.
- The chain completed with 450 `StOK`, 10 `StWarn`, 0 `StErr`, and 0
  `StFatal` returns.
- The output MuDst contains 10 entries, 56 branches, and is not marked as a
  recovered ROOT file.
- For 7 of 10 events, TPC used/good-hit counts, BTOF hit count, primary-vertex
  count, and BEMC/EEMC fired/track/match summaries all match the original log
  exactly.
- The remaining three events have small tracking differences: event 1084365
  differs by 6 used and 32 good TPC hits plus one BEMC-associated track;
  event 1084623 differs by one used and one good TPC hit; event 1084632
  differs by 4 used and 17 good TPC hits.

Regression evidence:

```text
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/validation_P16id_DAQ_10evt_agdgetp_fix_20260725/
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/SL16d_embed_64b/validation_build_agdgetp_20260725.log
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/SL16d_embed_64b/validation_build_agdgetp_20260725.status
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/SL16d_embed_64b/validation_noop_after_p16id_fixes_20260725.log
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/SL16d_embed_64b/validation_noop_after_p16id_fixes_20260725.status
```

This is a strong reconstruction-level compatibility check, but it is not a
substitute for the full histogram/plot comparison required to establish
physics equivalence of the 32-bit and 64-bit releases.

## Important validation distinction

The compiled copied release above was fully validated. The Git branch was
reconciled onto the newer Git representation of `SL16d_embed_v3`; it has passed
Git whitespace checks and Perl syntax checks for `mgr/ConsDefs.pm` and
`mgr/cons`, but a complete clean build directly from this Git checkout should
still be run before merging.
