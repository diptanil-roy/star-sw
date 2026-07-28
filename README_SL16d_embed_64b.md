# Local SL16d_embed 64-bit build

This directory is a copied and locally patched build of the centrally
installed STAR `SL16d_embed` release. It was ported from the original 32-bit
release to the SL7 64-bit target `sl73_x8664_gcc485` for execution from
AlmaLinux 9 `starsub` nodes through the STAR SL7 Singularity container.

## Location

```text
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/SL16d_embed_64b
```

Original central source:

```text
/star/nfs4/AFS/star/packages/SL16d_embed
```

## P16id floating-point compatibility correction (2026-07-27)

The final reconstruction-level mismatch was traced to the historical
`StTpcDb::DriftVelocity()` return boundary. The stored drift velocity is a
`Float_t`. On 32-bit i386, GCC returned the expression
`1e6*mDriftVel[kase]` through the x87 stack and downstream coordinate code
observed excess precision. On x86-64, the ABI rounded that expression to
`Float_t` in an XMM register before `StTpcCoordinateTransform` promoted it
back to `Double_t`.

The accepted correction is:

- `StRoot/StTpcDb/StTpcDb.h` and `.cxx`
  - retain the historical `Float_t DriftVelocity()` ABI;
  - add `Double_t DriftVelocityDouble()` for calculations that require the
    pre-64-bit expression precision.
- `StRoot/StDbUtilities/StTpcCoordinateTransform.cc`
  - use `DriftVelocityDouble()` at its four drift-velocity call sites.
- `mgr/Conscript-standard`
  - build `StDbUtilities` with `-mfpmath=387 -mpc80`, preserving the already
    validated x87 behavior in the rest of the coordinate calculation.

The first 100 events of the historical P16id DAQ test now match the original
production log in every compared run/event ID, TPC used/good-hit count,
global-track count, BTOF hit count, primary-vertex count, and BEMC/EEMC
summary. The two previously independent failures (event IDs `1085329` and
`1087338`) are exact after the correction.

A broader experiment rebuilding all 31 reconstruction packages with x87 was
rejected: it left exactly the same five mismatched comparison rows at events
`1085329` and `1087338`. This confirms that the decisive loss occurred at the
x86-64 `Float_t` function-return boundary, not in ordinary downstream SSE
arithmetic.

Evidence:

```text
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/diagnostics_20260727/double_drift_velocity_64/test_first100/comparison.csv
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/diagnostics_20260727/full_reco_x87_64/test_first100/comparison.csv
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/SL16d_embed_64b/full_cons_after_double_drift.log
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/SL16d_embed_64b/noop_cons_after_double_drift.log
```

## Source and build changes

### Build infrastructure

- `mgr/ConsDefs.pm`
  - Ported compiler, Fortran, linker, and dependency discovery to the SL7
    64-bit STAR environment.
  - Added STAR-spack-aware MySQL, log4cxx, XML, FastJet, GSL, Qt, `CPATH`, and
    `PYTHONPATH` handling.
  - Pointed `agetof` at the generated 64-bit definition file.
- `mgr/Construct`
  - Enabled GEANT3, GCALOR, StarSim, `root4star`, Qt, and online packages in a
    64-bit build instead of skipping the legacy targets.
- `mgr/cons`
  - Fixed relative `Conscript` loading for modern Perl by using `./...` when
    appropriate.
- `asps/rexe/Conscript`
  - Updated the 64-bit executable/link rules, including `root4star`.
- `QtRoot/Conscript`
  - Added the QtRoot build/install target.
- `local-qmake/qmake` and `local-qmake/qt.conf`
  - Relocate the old Qt4 `qmake` installation from its compiled-in AFS prefix
    to the accessible NFS4 Qt prefix.
- `local-bin/stic`
  - Exposes the legacy StAF `stic` IDL generator without mixing its 32-bit
    libraries into the 64-bit link environment.

### StarSim, GEANT3, and address-width fixes

- `asps/rexe/TGeant3/gcomad.F`
  - Uses pointer-width-aware address return types.
- `asps/Simulation/agetof/agetof.def`
  - Uses 64-bit displacement/address helpers and correct character-member
    displacement generation.
- `asps/Simulation/starsim/atgeant/agsbegm.age`
  - Replaces an address-derived RZ buffer with a fixed local buffer.
- `asps/Simulation/starsim/deccc/idisp.c`
  - Uses 64-bit displacement return values.
- `asps/Simulation/starsim/deccc/locf.c`
  - Adds pointer-token conversion helpers for addresses that cannot be stored
    directly in legacy 32-bit integer interfaces.
- `asps/Simulation/starsim/dzdoc/mzpaw.F`
  - Adjusts PAWC/ZEBRA memory handling for the 64-bit layout.
- `asps/Simulation/starsim/geant/gzebra.F`
  - Uses a fixed 64-bit-safe GEANT bank layout instead of address arithmetic
    through truncated integers.
- `StRoot/St_geant_Maker/navigate.g`
  - Stores geometry nodes and related addresses in 64-bit integers.
- `StRoot/St_geant_Maker/St_geant_Maker.cxx`
  - Uses pointer-width-safe GEANT volume/address interfaces and converts legacy
    address tokens back to pointers.

### ROOT 5 dictionary and Jevp cleanup

ROOT 5 `rootcint` could not parse the low-level RTS platform, pthread, and
VxWorks compatibility headers on 64-bit Linux. Implementation-only DAQ/RTS
includes were moved out of public dictionary headers, and forward declarations
were added where only pointer types are required.

Modified Jevp files:

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

### Other 64-bit C/C++ fixes

- `OnlTools/PDFUtil/PdfIndex.cxx`
  - Prints pointers with `%p` and uses `%zu` for `strlen()` results.
- `StRoot/RTS/trg/include/trgDataDefs.h`
  - Gives previously anonymous trigger structures stable names needed for
    dictionary-safe forward declarations.
- `StRoot/StSpinPool/StFmsTriggerMaker/Crate.hh`
  - Removes low-level RTS trigger definitions from public dictionary input and
    replaces them with forward declarations.
- `StRoot/StSpinPool/StFmsTriggerMaker/Crate.cxx`
  - Contains the implementation and low-level RTS includes moved out of the
    public header.
- `StRoot/StSpinPool/StFmsTriggerMaker/StFmsTriggerMaker.cxx`
  - Updated to use the separated crate implementation.

### Added generator IDL inputs

- `pams/gen/idl/eg_event.idl`
- `pams/gen/idl/eg_gener.idl`
- `pams/gen/idl/eg_track.idl`
- `pams/gen/idl/eg_vertex.idl`
- `pams/gen/idl/particle.idl`

## Non-source contents created during the port

- `.sl73_x8664_gcc485/`
  - Complete generated 64-bit object, library, include, and binary tree.
- `.sl73_x8664_gcc485.before_64b_port_20260713_172524/`
  - Preserved earlier build output from before the build-system port.
- `backup_20260713_172148/`, `backups/`, and files containing `.before_` or
  `.bak`
  - Preserved originals from before individual source changes.
- `find_jevp_platform_headers.py`, `find_jevp_rootcint_failures.py`, and
  `find_jevp_rootcint_all_errors.py`
  - Diagnostic scripts used to isolate ROOT dictionary failures by header.
- Build and validation logs
  - Retained as evidence; they are not part of the source port.

No existing source or backup was intentionally deleted during the port.

## Build environment

From an AlmaLinux 9 `starsub` node:

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

The 32-bit `stic` executable is only an IDL-to-source generator. Its generated
source is compiled and linked by the 64-bit toolchain.

## Validation

- Full `cons` completed without fatal build errors.
- A true no-op `cons` returned status `0` and ended with
  `cons: "." is up-to-date.`
- Local `root4star` is a 64-bit executable.
- `runBfc.C` processed all 10 events from
  `rcf22000_15117062_0000_1_10evts.fzd` successfully.
- The resulting MuDst contains exactly 10 entries.

No-op build evidence:

```text
validation_true_noop_20260718.log
validation_true_noop_20260718.status
```

Runtime validation output:

```text
/gpfs01/star/pwg/droy1/STAR-Workspace/D0Analysis/PythiaScript/validation_runBfc_SL16d_64b_final_20260718_2352/rcf22000_15117062_0000_1_10evts.MuDst.root
```

### P16id DAQ regression and BTOF TGeo backport

The original 32-bit P16id production log was used as a reference:

```text
/star/data05/scratch/starreco/P16id/prod/st_physics_16148020_raw_4000024.log.gz
```

The unpatched 64-bit port loaded the same BTOF DAQ hits but initialized only
5 of 120 BTOF trays through the legacy `TVolumeView` geometry path. This
reduced projected and matched BTOF tracks severely.

The runtime tree now uses STAR's later TGeo-based BTOF implementation from the
tree at upstream commit `d5f31ed1ddc3bffdd1842e74d283d44879f0c3f0`
(`Use TGeo initializer for BTof geometry`) in:

```text
StRoot/StBTofUtil/StBTofGeometry.cxx
StRoot/StBTofUtil/StBTofGeometry.h
StRoot/StBTofMatchMaker/StBTofMatchMaker.cxx
StRoot/StiMaker/StTrack2FastDetectorMatcher.cxx
```

The remaining old caller in `StRoot/StPeCMaker/StPeCMaker.cxx` was changed
from `InitFromStar(mstarHall)` to `Init(this, mstarHall)`.

After a full dependency rebuild:

- Both BTOF users initialize `120` valid trays with `32` modules per tray.
- All 10 reference run/event IDs are identical.
- BTOF DAQ-hit, matched-hit, valid-match, and primary-valid-match counts match
  the 32-bit P16id reference for every event.
- Seven of the 10 events match all selected tracking/detector metrics exactly.
- Three events retain small, deterministic Sti differences:
  - event `1084365`: used/good TPC hits `33784/30164` (reference) versus
    `33778/30132` (64-bit);
  - event `1084623`: `13546/12135` versus `13545/12134`;
  - event `1084632`: `11414/10062` versus `11418/10079`.
- Rebuilding all Sti packages with `-mfpmath=387` produced the same event
  metrics, so that experiment was rejected and the standard 64-bit compiler
  flags were restored.
- The accepted tree passed a final status-0 no-op build ending with
  `cons: "." is up-to-date.`

Accepted validation evidence:

```text
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/validation_P16id_DAQ_10evt_btof_tgeo_final_normal_20260727/
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/SL16d_embed_64b/build_final_noop_btof_tgeo_normal_20260727.log
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/SL16d_embed_64b/build_final_noop_btof_tgeo_normal_20260727.status
```

The pre-backport source files are preserved under:

```text
port_backups/20260726_btof_tgeo_firstpass/
```

## Access policy

The tree is owner-writable. Group and other users receive read access to files
and read/traverse access to directories; they do not receive write access.

## Recursive 32-bit compatibility pass (2026-07-27)

After the drift-velocity correction, reconstruction was compared recursively
against a fresh 32-bit `SL16d_embed` build using all 19,360 events in
`st_physics_16148020_raw_4000024.daq`. The accepted Sti correction supersedes
the earlier package-wide x87 experiment described above:

- `mgr/Conscript-standard` builds the `Sti` package with `-mfpmath=387` on
  `x8664`.
- `StRoot/Sti/StiTrackNodeHelper.cxx` gives only
  `StiTrackNodeHelper::propagateMtx()` a narrow
  `#pragma GCC target("sse2,fpmath=sse")` override.

This mixed mode retains the historical x87 behavior through the track fit
while keeping covariance propagation in the 53-bit SSE mode that best
reproduces the 32-bit reference. The accepted object was rebuilt from the
actual source tree with `cons`; the `.text` section of
`StiTrackNodeHelper.o` is byte-identical to the independently tested
candidate object.

The recursive search included:

- plain 64-bit SSE and package-wide Sti x87 baselines;
- every Sti root, `Base`, and `Star` object substituted individually;
- full `StiMaker` x87 and full `StGenericVertexMaker` x87 builds;
- GCC 4.8.2 and GCC 4.8.5 variants;
- x87 precision-control and narrow SSE function combinations.

No tested single-object or downstream package substitution improved the
remaining primary-track boundary cases. A physics-threshold change was not
accepted merely to force bitwise equality.

Full-sample results for the accepted mixed mode:

- STAR-aware histogram comparison: 1,372 histograms, 810 identical and 562
  different, with 42,340 different cells. This is better than both plain SSE
  (775 identical) and package-wide Sti x87 (745 identical).
- MuDst structural scan: 404 mismatch events, versus 439 for plain SSE and
  980 for package-wide Sti x87.
- Log-level coarse triage: 178 candidate-only mismatch events, versus 205 for
  plain SSE.

The accepted source tree was then rebuilt cleanly and validated without an
overlay:

- the first 10 DAQ events match every compared P16id metric;
- the first 104 DAQ events match every compared run/event ID, used and good
  TPC-hit count, global-track count, BTOF count, primary-vertex count, and
  BEMC/EEMC summary;
- the 104-event MuDst structural comparison has three residual mismatch
  events (positions 8, 54, and 104). At position 104 the global-track
  structure is identical and only the primary-track hash differs;
- `runBfc.C` processes all 10 events in
  `rcf22000_15117062_0000_1_10evts.fzd`;
- the resulting MuDst is not recovered, has 56 branches, and contains exactly
  10 entries;
- `root4star` and `Sti.so` have no unresolved shared libraries in the
  initialized SL7 runtime;
- the final whole-tree `cons` returns status 0 with
  `cons: "." is up-to-date.` and issues no compiler, linker, generator, or
  make commands.

Evidence:

```text
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/diagnostics_20260727/rebuild_accepted_sti_minimal.log
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/diagnostics_20260727/accepted_full_cons_after_patch.log
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/diagnostics_20260727/accepted_full_noop_cons.log
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/diagnostics_20260727/accepted_daq10_final/comparison.csv
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/diagnostics_20260727/accepted_daq104_final/comparison.csv
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/diagnostics_20260727/accepted_daq104_structure_vs_exact32.csv
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/diagnostics_20260727/hist_exact32_vs_mixed.csv
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/diagnostics_20260727/mixed_structure_full/
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/diagnostics_20260727/accepted_fzd10_final/
/gpfs01/star/pwg/droy1/STAR-Workspace/LocalSTAR/diagnostics_20260727/accepted_runtime_linkage.txt
```
