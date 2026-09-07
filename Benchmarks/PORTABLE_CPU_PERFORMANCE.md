# Portable CPU performance report

> **This file is a historical record (PRs #329-#333) and is no longer
> appended to.** New performance/validation write-ups go in their own file
> under [`Benchmarks/reports/`](reports/README.md), one per branch, to avoid
> the repeated merge conflicts this file accumulated from parallel branches
> editing its tail.

## Scope and result

This series targets the native C++ network-generation path used by production
models in `bng2/Models2`. It does not optimize a synthetic microbenchmark, alter
the CLI, change generated-file formats, or add an external dependency.

The first retained optimization is the two-commit implementation in `533ac26b`
(`perf: skip canonical labels for exact product duplicates`) and `92ca4c03`
(`perf: preserve canonical product ordering in exact dedup fast path`). On the
paired production matrix, its median end-to-end wall-time changes ranged from
-0.172% to -6.903%, and median CPU-time changes ranged from +0.081% to
-7.288%. The small `blbr` case is close enough to the noise floor that it is
reported as a workload limitation; the medium and large cases provide the
material evidence for retaining that change.

The second retained optimization is `7ee2db11` (`perf: cache immutable
reaction pattern metadata`). Against the first retained candidate, its median
CPU-time change was negative on all four workloads, ranging from -0.856% to
-2.317%; the detailed second matrix and its spread are recorded below.

The branch also contains two correctness/build-enablement fixes and focused
tests:

* `77c8bd8c` constrains legacy `BNGcore` inequality overloads to BNGcore types,
  fixing an Apple Clang/libc++ ADL ambiguity.
* `46da45c4` defines empty pattern-graph canonicalization and adds its test.
* `5291159d` tests that compartment-aware species remain distinct while exact
  duplicates still deduplicate.

## Repository and source provenance

The repository was synchronized before benchmarking:

```text
fork:     https://github.com/akutuva21/bionetgen.git
upstream: https://github.com/RuleWorld/bionetgen.git
upstream default discovered with git ls-remote --symref: master
upstream/master at synchronization: 43ddf3afe165192a222fd13e4917a1902ffe3446
origin/master at synchronization:   b00410628484f639efbf294f8a150f21c4e8bb29
working branch:                      codex/portable-cpu-20260831
baseline source:                     46da45c4
retained performance source:         92ca4c03, 7ee2db11
final source tree:                   7ee2db11
```

`git pull --ff-only origin master` was run before creating the dedicated
branch. At the pre-cache candidate-source audit, the branch was six commits
ahead of `origin/master` and nine commits ahead of `upstream/master`. A final
local/remote audit was performed after the source and documentation commits;
the delivered branch ref, divergence, and CI/PR query results are reported in
the delivery summary. The previously dirty files and explicitly listed
untracked file were cleared at the user's request before this series began;
the final tree is required to be clean.

The final performance source includes the rejected exact-key experiment
`70acc9e2` followed by `e08a9bd2`, which restores the retained implementation,
and the accepted immutable pattern-metadata cache `7ee2db11`. The rejected
experiment is documented below and is not present in the final source behavior.

Input SHA-256 hashes:

| Model | SHA-256 |
| --- | --- |
| `bng2/Models2/blbr.bngl` | `c3290588efecbd3be2e57d883e851d6b28edd0c39eed43a491aa5c20533b6e96` |
| `bng2/Models2/SHP2_base_model.bngl` | `790d51dc260f9b3da7cd214ea6290998c81ed4ab4bd2f2e0dd8f16c49eb8182a` |
| `bng2/Models2/egfr_net.bngl` | `843e07954d5dfb1acc99e294a2d7518b41f4813992567edc2c47794cec10a1da` |
| `bng2/Models2/fceri_ji.bngl` | `fa31fe7375510bb3e05f2335719a71abc1a22ada75be3eb5a8d5a0c80f29227e` |

## Hardware and build configuration

```text
OS:             macOS 26.6.2, Darwin 25.6.0, arm64
Hardware:       Mac17,9; 15 logical CPUs; 24 GiB RAM
C++ compiler:   /usr/bin/clang++ Apple clang 21.0.0 (clang-2100.1.1.101)
CMake:          4.4.3
Python:         3.9.6
Perl:           5.34.1
Build type:     Release
Architectures:  arm64
Release flags:  -O3 -DNDEBUG
Dependencies:   Catch2 v3.4.0, ANTLR4 4.13.1, SUNDIALS v7.6.0
SUNDIALS index: 64-bit; static dependency builds
```

Release configuration and build:

```sh
cmake -S . -B build-codex \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_OSX_ARCHITECTURES=arm64 \
  -DBUILD_TESTING=ON
cmake --build build-codex --parallel 4
```

## Workloads and benchmark protocol

The model actions exercise native parsing, network generation, and—where the
model defines them—deterministic ODE/steady-state execution:

| Workload | Role | Model action | Generated network |
| --- | --- | --- | --- |
| `blbr` | small/noise check | network generation | 20 species, 118 reactions |
| `SHP2_base_model` | medium | network generation, ODE 1000/100, steady state | 149 species, 1082 reactions |
| `egfr_net` | large | network generation, ODE 40/50 | 356 species, 3749 reactions |
| `fceri_ji` | large | network generation, ODE 600/10 | 354 species, 3680 reactions |

No stochastic or SSA action is present in this matrix, so there was no RNG
seed or stochastic event signature to compare. This is a limitation of this
target selection, not a claim of stochastic validation.

The committed runner is
`Benchmarks/portable_cpu_benchmark.py`. It starts a fresh worker process and
fresh run directory for every executable invocation, copies the pinned model,
alternates baseline/candidate order on each pair, and records raw JSON for
wall time, user/system CPU time, maximum RSS, output sizes and hashes, network
species/reaction counts, data-row counts, command lines, and input/executable
hashes. Twenty paired repetitions were used per model:

```sh
python3 Benchmarks/portable_cpu_benchmark.py \
  --executable-a /private/tmp/bng_cpp-baseline-46da45c4 \
  --executable-b /private/tmp/bng_cpp-candidate-exact-precheck-move \
  --model bng2/Models2/blbr.bngl \
  --model bng2/Models2/SHP2_base_model.bngl \
  --model bng2/Models2/egfr_net.bngl \
  --model bng2/Models2/fceri_ji.bngl \
  --repetitions 20 --timeout 30 \
  --output /private/tmp/portable_cpu_exact_precheck_move_20.json
```

The first retained Release executable had the same SHA-256 as the candidate
artifact used above. The baseline executable SHA-256 was
`ece7a09b247a2a8eb95dbaa26db7427aa0e5d0772f5b7b8097a6d36aa404399a`; the
first retained candidate executable SHA-256 was
`a59ed4dc4ff5becf347b95f1915c6ddaf60bc54d4bb84a0160ae60a737d2a9a2`.
The final Release executable, including the metadata cache, is
`build-codex/src/bng_cpp` with SHA-256
`c4eeb05df99869d34364671089df598c7ecfc5f700c70e957297e968cb9b4d15`.
CPU time and RSS use Python `resource.getrusage` because `/usr/bin/time -l`
cannot read the required macOS counters in the sandbox.

The percentage is `100 * (candidate - baseline) / baseline`; negative means
the candidate is faster. The table reports independent medians and paired
median deltas. IQR and min/max are the paired percentage spread.

| Workload | Outputs: bytes; data rows | Wall baseline -> candidate (s) | Paired wall delta: median [IQR; min..max] | CPU baseline -> candidate (s) | Paired CPU delta: median [IQR; min..max] |
| --- | ---: | ---: | ---: | ---: | ---: |
| `blbr` | 5,112; N/A | 0.024501 -> 0.024358 | -0.172% [-0.832..+0.522; -6.923..+1.550] | 0.021025 -> 0.020930 | +0.081% [-0.961..+0.636; -7.255..+1.262] |
| `SHP2_base_model` | 343,261; 202 | 0.164564 -> 0.153212 | -6.903% [-7.483..-6.295; -8.662..-5.533] | 0.158193 -> 0.147044 | -7.288% [-7.528..-6.808; -8.215..-5.269] |
| `egfr_net` | 549,606; 102 | 0.582642 -> 0.566102 | -2.985% [-3.502..-2.451; -5.289..-1.055] | 0.570982 -> 0.554018 | -3.210% [-3.531..-2.240; -5.084..-1.687] |
| `fceri_ji` | 233,656; 22 | 0.665730 -> 0.638934 | -3.938% [-4.338..-3.769; -5.251..-2.903] | 0.653576 -> 0.627741 | -3.956% [-4.284..-3.753; -5.625..-3.032] |

Paired wall/CPU wins were respectively 11/20 and 11/20 for `blbr`, 20/20
and 20/20 for `SHP2`, 20/20 and 20/20 for `egfr_net`, and 20/20 and 20/20
for `fceri_ji`. Median maximum-RSS deltas were -0.146%, -0.610%, +0.379%,
and +0.062% in the same workload order. The medium and large workloads
therefore show repeatable CPU and wall-time reductions; `blbr` is retained
only as a small correctness/noise check, not as the performance justification.

The second retained optimization was measured against the first retained safe
candidate (`a59ed4dc...`) with 40 paired repetitions per workload (320 raw
records). The candidate executable was
`c4eeb05df99869d34364671089df598c7ecfc5f700c70e957297e968cb9b4d15`. The raw
result is `/private/tmp/portable_cpu_pattern_metadata_cache_40.json`.

```sh
python3 Benchmarks/portable_cpu_benchmark.py \
  --executable-a /private/tmp/bng_cpp-candidate-exact-precheck-move \
  --executable-b build-codex/src/bng_cpp \
  --model bng2/Models2/blbr.bngl \
  --model bng2/Models2/SHP2_base_model.bngl \
  --model bng2/Models2/egfr_net.bngl \
  --model bng2/Models2/fceri_ji.bngl \
  --repetitions 40 --timeout 30 \
  --output /private/tmp/portable_cpu_pattern_metadata_cache_40.json
```

| Workload | Wall retained -> cache (s) | Paired wall delta: median [IQR; min..max] | CPU retained -> cache (s) | Paired CPU delta: median [IQR; min..max] |
| --- | ---: | ---: | ---: | ---: |
| `blbr` | 0.023026 -> 0.022553 | -2.197% [-3.642..+0.358; -8.767..+4.164] | 0.022000 -> 0.021540 | -2.317% [-3.590..+0.368; -8.756..+4.781] |
| `SHP2_base_model` | 0.151441 -> 0.150011 | -0.720% [-1.954..+0.503; -4.262..+2.893] | 0.149507 -> 0.148274 | -0.856% [-1.927..+0.131; -3.838..+2.036] |
| `egfr_net` | 0.568566 -> 0.566068 | -0.997% [-1.621..+0.010; -4.814..+4.013] | 0.565503 -> 0.562712 | -1.014% [-1.633..-0.076; -4.098..+4.055] |
| `fceri_ji` | 0.637187 -> 0.631220 | -1.108% [-1.396..-0.436; -5.827..+0.483] | 0.633409 -> 0.626914 | -1.116% [-1.435..-0.593; -5.985..+0.042] |

Paired wall/CPU wins in this second run were 29/40 and 29/40 for `blbr`,
27/40 and 29/40 for `SHP2`, 29/40 and 31/40 for `egfr_net`, and 38/40 and
39/40 for `fceri_ji`. Median maximum-RSS deltas were -0.438%, +0.989%,
0.000%, and +0.372%, respectively. All four CPU medians improved; the
medium workload's IQR crosses zero slightly, so this records the consistent
direction and artifact identity without claiming every individual pair wins.

## Mechanism changed

`src/ast/SpeciesList.cpp` now exposes an exact-key lookup that checks the
compartment-aware string index without canonicalizing the graph. The existing
`add` path also checks that exact key before canonical labeling. Canonical
labeling remains in the fallback path. For species with a species-level
compartment, the deduplication string is recomputed after canonicalization
because serialization can depend on canonical node ordering; unscoped species
retain the pre-label exact key and existing canonical-label and
structural-fingerprint fallbacks.

`src/ast/ReactionRule.cpp` moves each product graph into a temporary `Species`,
probes the exact key, and returns the existing cached label immediately for an
exact duplicate. Only an exact-key miss canonicalizes the owned graph before
insertion, preserving the original canonical product ordering for non-duplicate
products. Labels are then read from the stored graph, preserving reaction
sorting and output. This ordering is important: a follow-up that carried a
pre-canonical exact key into insertion timed out on `Motivating_example_cBNGL`
because canonical labeling can change serializer order; its safety-gated
variant was noise-level or slower and was not retained.

`src/ast/ReactionRule.hpp` and `src/ast/ReactionRule.cpp` add a per-rule cache
of the immutable `PatternInfo` descriptions for reactant and product graphs.
The cache is built during `initialize()` and reused by embedding searches,
reaction construction, and delete-molecule handling, eliminating repeated
metadata allocations without changing graph ownership, match ordering, or
operation data. The out-of-line destructor and move operations keep the
opaque cache type portable across translation units.

The focused regression in `tests/ast/test_SpeciesList.cpp` covers two species
with identical structure but different molecule compartments, exact duplicate
reuse, and the resulting list size. The empty-graph test covers the
canonicalization guard needed by the validated build. The existing
`tests/ast/test_network_generator.cpp` path also reinitializes and moves a
`ReactionRule` into a model before checking generated species/reaction counts,
covering the cached rule metadata's lifecycle.

## Correctness evidence

For all 20 repetitions of all four workloads in the first retained comparison,
the candidate and baseline output-hash maps were identical. For all 40
repetitions in the metadata-cache comparison, the cache candidate and the
retained executable also had identical output-hash maps, sizes, and network
counts. The resulting deterministic artifacts had these stable hashes:

```text
blbr.net                         f983ce459044a975daa8ddf68b74cba694117cc56f03047dfadee4c904795bc2
SHP2_base_model.net              6620f31870265eff428152ed30940c5ca2e282205f1b0d438cce61ea7f7148f3
SHP2_base_model.cdat             05528472a0c0c48d88929651b6b37123d9e2c6c0b109a8bd9d5a512c222da95b
SHP2_base_model.gdat             542fd2ba1e4117b809b38fb8d0c836c26a827d8e4a6ea9edcd3f94038068f957
egfr_net.net                     84dce91d99d292092ec89878103d2f1ff6819d9f08415937728e733cd0a4eb71
egfr_net.cdat                    352544d406b91466d30494e59f5df228ae983b002221a6a53fd000bbd49c048b
egfr_net.gdat                    4ea6425ae6e3e9a7ebcf7336d5454e6fc12fe78cdadf8c6d8fb3da85ca7de439
fceri_ji.net                     f0cc7a523f6b8376bda9de1f3306fea9722b682559d33f05fd4dd2fd6107ad73
fceri_ji.cdat                    b2caf3891093c858af935a420a5331512ee9b11a1383fa9f4d701915ba125a3b
fceri_ji.gdat                    a3bb8268b0de2294aec732dddefc93def96a500a334f0a4d756e7e894effa5bc
```

Independent BNG2 reference generation used the checked-out Perl implementation
(`bng2/BNG2.pl`, reported version 2.9.3, SHA-256
`cf5fd82d3df9b84835d29234bd32268b87eaa985dd221bd5d39aadef744795f4`). The
reference command was `perl bng2/BNG2.pl <copied-model>.bngl`, followed by the
repository validators:

```sh
perl bng2/Validate/compare_species.pl -n <candidate.net> <reference.net>
perl bng2/Validate/compare_rxn.pl <candidate.net> <reference.net>
```

Results were:

| Workload | Candidate network | Independent BNG2 network | Species comparison | Reaction comparison |
| --- | ---: | ---: | --- | --- |
| `blbr` | 20 / 118 | 20 / 92 | pass | fail: pre-existing reaction-count mismatch |
| `SHP2_base_model` | 149 / 1082 | 149 / 1032 | pass | fail: pre-existing reaction-count mismatch |
| `egfr_net` | 356 / 3749 | 356 / 3749 | pass | pass |
| `fceri_ji` | 354 / 3680 | 354 / 3680 | pass | pass |

Counts are species/reactions. The two small/reference mismatches were present
in the baseline C++ behavior before this optimization and are not introduced
by the candidate; candidate-vs-baseline artifacts remain byte-identical. The
repository `compare_rxn.pl` also emits a pre-existing missing-argument warning
and prints a misleading `0 versus 0` count in its failure message; the counts
above were independently parsed from both `.net` files.

## Profiles and rejected alternatives

The baseline macOS `sample` profile for repeated EGFR network-generation runs
captured 5 seconds and 5,648 samples. The dominant stack was
`ActionDispatch::execute` (3,295), `NetworkGenerator::generate` (3,015),
`NetworkGenerator::generateNative` (3,013), and
`ReactionRule::expandRule` (1,175). `SpeciesList::add`, canonical labeling,
`find_canonical_order`, and BNG2-string serialization were visible below that
stack. A direct final-candidate EGFR profile captured the same stack during
the run: `ActionDispatch::execute` (248 samples),
`NetworkGenerator::generateNative` (223), `ReactionRule::expandRule` (81),
`SpeciesList::add` (at least 9 samples in its top path), and canonical-label
calls nested in product construction. The FcERI profile likewise showed
network generation as the relevant generation hotspot, while its full run was
also dominated by the ODE solver.

Measured alternatives were not retained. The node-index edge-lookup experiment
used executable SHA-256
`de090c072b46654105f4a7b2bd4a2b62559b08f3cb5fd9ec41b8a14f942fd471`, and the
serializer-key experiment used executable SHA-256
`2f2bcd847bcb13ebb420fbf38fd386dfab78f5d9bace103cc6cd1eeb2dd2e930`. Both
were compared directly with the retained candidate:

| Experiment | Paired deltas by `SHP2`, `blbr`, `egfr_net`, `fceri_ji` | Decision |
| --- | --- | --- |
| `std::map<Node*, int>` -> unordered map in canonicalization | +1.009%, -1.365%, +0.232%, -0.281% | reject: inconsistent and slower on two larger cases |
| Exact-first without the compartment-safe follow-up | -4.611%, +5.299%, +1.444%, -1.441% | reject: regressions on `blbr` and EGFR |
| Fingerprint-gated serialization | -1.982%, +9.640%, +3.199%, +0.407% | reject: broad regressions |
| Fingerprint plus isomorphism precheck | +10.907%, +30.282%, +22.450%, +9.928% | reject: decisively slower |
| Temporary node-index edge lookup (wall / CPU) | +0.205% / +0.217%, -0.600% / -1.554%, -0.928% / -1.006%, -0.964% / -1.003% | reject: SHP2 CPU regression; only 9/20 CPU wins |
| Cached compartment-aware serializer keys (wall / CPU) | +0.066% / +0.061%, -0.071% / +0.118%, +0.529% / +0.509%, +0.748% / +0.719% | reject: EGFR and FcERI regressions; 7/20 and 4/20 CPU wins |
| Exact-key handoff after product canonicalization | -0.885% / -0.836%, -5.938% / -6.848%, -4.592% / -4.395%, -1.648% / -1.667% | reject: timed out on `Motivating_example_cBNGL` in the 41-model harness |
| Safety-gated exact-key handoff (canonical graphs only) | +0.625% / +0.513%, +0.078% / -0.031%, +0.548% / +0.637%, -0.188% / -0.193% | reject: noise-level/slower; no material matrix win |
| Fingerprint lookup before canonical labeling | -0.535% / -0.541%, +3.241% / +4.009%, +0.133% / +0.195%, -0.460% / -0.366% | reject: `blbr` CPU regression; no broad win |

The remaining profile-dominant CPU work after both retained changes is
canonical labeling/Nauty and related graph/string operations inside rule
expansion and species deduplication. ODE integration is another dominant cost
for ODE-heavy models such as FcERI. A further material gain in those areas
would require a broader canonicalization and deduplication data-structure
redesign, parallel/GPU execution, or a different-language implementation;
those are outside this portable, semantics-preserving scope.

## Expanded-scope graph/string result

Branch `codex/graph-string-20260901` is based directly on CPU tip
`305b7482febe3dd52ccd517fa4cd2e02504e834c` and is pushed only to the fork. The
validated source commit is `556099d3` (`perf: reduce BNG2 graph string
allocations`). `src/core/Node.cpp:290-422` changes the recursive BNG2 graph
serializer from temporary `std::stringstream` objects to `std::string`
append operations and `std::to_string`. It leaves edge sorting, bond numbering,
canonical indices, compartment handling, and traversal order unchanged.
`tests/test_pattern_graph.cpp:78` strengthens the serializer regression to
require the exact `A(B)` artifact. No public API, CLI form, generated-file
format, or numerical behavior changed.

The production paired command was:

```sh
python3 Benchmarks/portable_cpu_benchmark.py \
  --executable-a /private/tmp/bng-cpu-base-305b7482 \
  --executable-b /private/tmp/bng-graph-string-builder \
  --model bng2/Models2/blbr.bngl \
  --model bng2/Models2/SHP2_base_model.bngl \
  --model bng2/Models2/egfr_net.bngl \
  --model bng2/Models2/fceri_ji.bngl \
  --repetitions 40 --timeout 30 \
  --output /private/tmp/portable_cpu_graph_string_builder_40.json
```

The baseline executable SHA-256 was
`c4eeb05df99869d34364671089df598c7ecfc5f700c70e957297e968cb9b4d15`; the
candidate SHA-256 was
`bdd7b6b590b77c10bd1c6a46e76645c5b5e76b3d03c815a12f3630e1a323e772`. The
runner used paired fresh processes/directories with alternating order and
recorded wall/user/system CPU time, maximum RSS, output bytes and hashes,
network counts, data rows, input hashes, executable hashes, and command lines.
The table reports median baseline -> candidate seconds and paired
candidate-minus-baseline percentage with inclusive IQR; negative means
faster.

| Workload | Wall seconds; paired delta | CPU seconds; paired delta | Result |
| --- | --- | --- | --- |
| `blbr` | 0.024425 -> 0.023488; -3.612% [-5.171..-2.477] | 0.020522 -> 0.019635; -4.071% [-5.637..-3.043] | small correctness/noise check |
| `SHP2_base_model` | 0.154169 -> 0.152491; -1.411% [-2.040..-0.463] | 0.146467 -> 0.144727; -1.321% [-1.643..-0.694] | repeatable |
| `egfr_net` | 0.569308 -> 0.554045; -2.930% [-3.681..-2.028] | 0.553046 -> 0.538493; -2.676% [-3.459..-2.184] | retain |
| `fceri_ji` | 0.638793 -> 0.630779; -1.201% [-1.944..-0.805] | 0.624821 -> 0.616700; -1.410% [-1.856..-0.917] | retain |

The graph/string candidate was faster in 39/40 wall and 40/40 CPU pairs for
`blbr`, 34/40 wall and 38/40 CPU pairs for SHP2, 40/40 wall and CPU pairs for
EGFR, and 34/40 wall and 35/40 CPU pairs for FcERI. All 40 repetitions for all
four workloads had identical candidate/baseline output-hash maps, output
sizes, and network counts. Median maximum-RSS changes were -0.146%, -0.046%,
+0.478%, and -0.215% in workload order. The deterministic artifacts are
therefore byte-identical while the large production models show material
CPU-time reductions.

The baseline macOS `sample` profile captured 12 copies of EGFR for 5 seconds
(`/private/tmp/bng-canonical-profile.iZwsyJ/profile.txt`); its top stacks were
`ActionDispatch::execute` (2,523),
`NetworkGenerator::generateNative` (2,261),
`ReactionRule::expandRule` (887), and `buildReaction` (193). Canonical graph
and string routines were visible inside those stacks:
`PatternGraph::get_label` about 59/56 samples across its top paths,
`find_canonical_order` about 26, `UllmannSGIso::refine_M` 41, and multiple
`Node::get_BNG2_string` paths. A unique-initial-color early return in
`PatternGraph::find_canonical_order` was tested on the same production matrix
and rejected: its 20-pair wall deltas were +0.843%, +0.841%, +0.681%, +0.710%
and CPU deltas +1.185%, +0.566%, +0.573%, +0.698% for
`blbr`, SHP2, EGFR, and FcERI respectively. It was removed and never
committed.

The fresh Release build at `/private/tmp/bng-final-graph-20260901` passed
CTest 80/80 and produced the candidate SHA above. The graph-branch
ASan/UBSan build passed its 80-test suite, the focused pattern-graph and ODE
tests passed, and all four production models exited without sanitizer
diagnostics. The repository's full 41-model harness was rerun and produced 34
passes and the same seven pre-existing environment/reference failures listed
earlier. Independent Perl BNG2 network checks preserve the existing
SHP2/blbr reaction-count discrepancies and pass for EGFR/FcERI; they do not
show a graph/string artifact change.

The remaining profile-dominant work is the canonical labeling/Nauty and
graph-deduplication machinery itself. The rejected early return demonstrates
that a local heuristic does not pay for its guard cost. A material next gain
requires a representation-level redesign that reuses canonical certificates or
batch-deduplicates graphs while preserving BNG2 ordering and collision-safe
semantics. Parallel/GPU execution or a different-language implementation would
also require a broader design; none is folded into this portable CPU branch.

## Canonical-label follow-up

Branch `codex/canonical-redesign-20260901` starts at the graph/string tip
`62f4dc6bd2a191d89a593e2d952e6c74c5b47271` and is pushed only to the fork. The
retained source commit is `b73d9e3d` (`perf: streamline canonical node
labels`). `src/core/Node.cpp:190-199` replaces the temporary
`std::stringstream` in `Node::get_label()` with one `std::string` assembled by
append and `std::to_string`. Canonical indices, node-type/state labels, graph
ordering, and all downstream formats are unchanged. The focused regression in
`tests/test_pattern_graph.cpp:213-219` requires the exact `7:A~<0>` label.

The candidate was compared with the retained graph/string executable using 40
paired repetitions per production model. The input hashes, commands, fresh
worker processes, alternating order, output measurements, and timeout were the
same as the graph/string run above; raw results are in
`/private/tmp/portable_cpu_node_label_40.json`:

```sh
python3 Benchmarks/portable_cpu_benchmark.py \
  --executable-a /private/tmp/bng-graph-string-builder \
  --executable-b /private/tmp/bng-canonical-direct-index/src/bng_cpp \
  --model bng2/Models2/blbr.bngl \
  --model bng2/Models2/SHP2_base_model.bngl \
  --model bng2/Models2/egfr_net.bngl \
  --model bng2/Models2/fceri_ji.bngl \
  --repetitions 40 --timeout 30 \
  --output /private/tmp/portable_cpu_node_label_40.json
```

The baseline executable SHA-256 was
`bdd7b6b590b77c10bd1c6a46e76645c5b5e76b3d03c815a12f3630e1a323e772`; the
candidate SHA-256 was
`9675de22ee67be6e2e5031ad815c6d033e9fe540ee07457043b334a19d533039`. The
table reports median baseline -> candidate seconds and paired median
candidate-minus-baseline reduction, where positive means faster. IQR is the
inclusive paired reduction spread.

| Workload | Wall seconds; reduction | CPU seconds; reduction | Result |
| --- | --- | --- | --- |
| `blbr` | 0.021886 -> 0.021758; +0.588% [-1.043..+2.933] | 0.019167 -> 0.018891; +1.443% [-0.996..+2.852] | small correctness/noise check |
| `SHP2_base_model` | 0.149267 -> 0.148588; +0.455% [-1.492..+1.571] | 0.143792 -> 0.143190; +0.419% [-1.229..+1.626] | small but positive |
| `egfr_net` | 0.554846 -> 0.546493; +1.505% [+0.492..+2.931] | 0.542507 -> 0.532942; +1.763% [+0.618..+2.862] | retain |
| `fceri_ji` | 0.630641 -> 0.621653; +1.425% [+0.552..+2.547] | 0.617730 -> 0.609962; +1.258% [+0.562..+2.484] | retain |

Wall/CPU wins were 24/40 and 23/40 for `blbr`, 26/40 and 24/40 for SHP2,
33/40 and 36/40 for EGFR, and 33/40 and 34/40 for FcERI. Median maximum-RSS
changes were +0.584%, +0.374%, +0.150%, and +0.184%. Every pair on every
workload had identical output-hash maps, output sizes, and network
species/reaction counts. The focused pattern-graph test passed 40 assertions
in 7 cases; the fresh Release build at
`/private/tmp/bng-final-canonical-20260901` passed CTest 80/80, and its
ASan/UBSan build plus all four production runs completed without diagnostics.

Several broader local-index shortcuts were screened and removed: a gated
node-index lookup regressed EGFR by 0.968% wall and 0.925% CPU while giving
mixed results elsewhere, and an edge-vector fast path was within noise or
slower on the larger models. The retained label-builder change is therefore a
small implementation improvement around the measured canonical hotspot, not a
claim that Nauty/canonical deduplication has been solved.

The remaining dominant canonical/Nauty work is the graph deduplication data
structure and repeated canonical-certificate construction inside rule
expansion. A material portable gain requires a broader representation-level
redesign that can reuse collision-safe certificates or batch-deduplicate
graphs while preserving BNG2 ordering. Process-level parallel work and the
GPU experiment remain on separate opt-in branches; the GPU path was not
retained because the required double-precision Metal kernel was not portable
to the available toolchain. A language rewrite is likewise out of scope.

## Current hotspot re-audit

The final canonical executable was re-profiled on the pinned production
`egfr_net` model on 2026-09-01 using a fresh 2-second macOS `sample` capture
(`/private/tmp/bng-hotspot-recheck.KqmjIE/canonical-egfr.sample`). The top
stack was `ActionDispatch::execute` (307 samples),
`NetworkGenerator::generate`/`generateNative` (278), and
`ReactionRule::expandRule` (106). Canonical work remained visible below it:
`PatternGraph::get_label`, `find_canonical_order`, `Node::get_label`, and
`PatternGraph::get_BNG2_string`/edge sorting. The run produced the pinned
EGFR network hash `84dce91d99d292092ec89878103d2f1ff6819d9f08415937728e733cd0a4eb71`.

A fresh 10-pair all-model sanity recheck used the same runner and paired
process protocol as the 40-pair result above. Reductions are positive when
the candidate is faster; all four models had equal output hashes, sizes, and
network counts:

| Workload | Canonical wall reduction | Canonical CPU reduction |
| --- | ---: | ---: |
| `blbr` | +1.294% | +1.566% |
| `SHP2_base_model` | +1.205% | +1.284% |
| `egfr_net` | +1.135% | +1.278% |
| `fceri_ji` | +0.759% | +0.776% |

This short recheck confirms that the branch/toolchain still exercises the
same canonical path; the earlier 40-pair matrix remains the performance
decision because the small sample is not a replacement for its spread.

## Validation commands and status

Targeted tests and the full Release CTest suite were run on the candidate:

```sh
build-codex/tests/test_SpeciesList
build-codex/tests/test_pattern_graph
build-codex/tests/test_network_generator
ctest --test-dir build-codex --output-on-failure --parallel 4
```

The targeted tests passed repeatedly, and the full suite passed 80/80. The
ASan/UBSan build used the same source and pinned local dependency trees:

```sh
cmake -S . -B /private/tmp/bng-build-asan \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang \
  -DCMAKE_OSX_ARCHITECTURES=arm64 -DBUILD_TESTING=ON \
  -DCMAKE_C_FLAGS='-fsanitize=address,undefined -fno-omit-frame-pointer' \
  -DCMAKE_CXX_FLAGS='-fsanitize=address,undefined -fno-omit-frame-pointer' \
  -DCMAKE_EXE_LINKER_FLAGS='-fsanitize=address,undefined' \
  -DCMAKE_SHARED_LINKER_FLAGS='-fsanitize=address,undefined' \
  -DFETCHCONTENT_SOURCE_DIR_CATCH2="$PWD/build/_deps/catch2-src" \
  -DFETCHCONTENT_SOURCE_DIR_ANTLR4_RUNTIME="$PWD/build/_deps/antlr4_runtime-src" \
  -DFETCHCONTENT_SOURCE_DIR_SUNDIALS="$PWD/build/_deps/sundials-src"
cmake --build /private/tmp/bng-build-asan --parallel 4
ctest --test-dir /private/tmp/bng-build-asan --output-on-failure --parallel 4
```

The sanitizer suite passed 80/80, and separate ASan/UBSan production runs of
all four models exited successfully without sanitizer diagnostics. No GitHub
Actions run can be created for this branch under the current workflow triggers:
they run on `master` pushes or pull requests, and the user explicitly
requested no RuleWorld pull request and no fork-default-branch push.

The repository script `scripts/validate_cpp_against_perl.sh` was also run over
all 41 available reference models for the final source using a temporary
macOS-compatible `timeout` shim (GNU `timeout` is not installed here). It
produced 34 passes and 7 failures, with the same failure set documented above:
the SHP2/blbr reaction-count mismatches, missing NFsim for
`isingspin_localfcn`, missing companion `.net` for `michment_cont`, unsupported
XML `readFile` in two SBML cases, and the missing `f_correct` parameter in
`test_time`. The previously passing `Motivating_example_cBNGL` also passed;
the rejected exact-key experiment had timed out there. These are reported
rather than silently treated as green.

No GitHub Actions run was created for the final branch SHA. The checked-in
workflows trigger on `master` pushes or pull requests, while the user
explicitly directed that no pull request be opened against `RuleWorld` and no
fork default-branch push be made. The final branch was pushed only to the
fork; local Release, independent-reference, full-harness, and ASan/UBSan
evidence above are the available validation for this no-PR delivery.


## Expanded-scope ODE integration result

Branch `codex/ode-integration-20260901` is based directly on CPU tip
`305b7482febe3dd52ccd517fa4cd2e02504e834c` and is pushed only to the fork. The
validated commit is `dd665873` (`perf: flatten large ODE reaction updates`).
`src/engine/OdeIntegrator.hpp:152-179` adds compact reaction metadata and
contiguous reactant/product index storage. `src/engine/OdeIntegrator.cpp:764-807`
packs only constant-rate networks with at least 512 reactions; smaller or
functional networks keep the prior representation. The hot loop at
`src/engine/OdeIntegrator.cpp:1016-1053` traverses the packed arrays in the
original reaction order, retaining double-precision multiplication and update
ordering. `tests/test_ode_integrator.cpp:170-198` covers the 512-reaction,
two-reactant/two-product boundary and exact derivative signs/magnitudes. No
public API, CLI, output format, or numerical option changed.

The production paired command was:

```sh
python3 Benchmarks/portable_cpu_benchmark.py \
  --executable-a /private/tmp/bng-cpu-base-305b7482 \
  --executable-b /private/tmp/bng-ode-flat-gated-reserved \
  --model bng2/Models2/blbr.bngl \
  --model bng2/Models2/SHP2_base_model.bngl \
  --model bng2/Models2/egfr_net.bngl \
  --model bng2/Models2/fceri_ji.bngl \
  --repetitions 40 --timeout 30 \
  --output /private/tmp/portable_cpu_ode_flat_gated_reserved_40.json
```

The baseline executable SHA-256 was
`c4eeb05df99869d34364671089df598c7ecfc5f700c70e957297e968cb9b4d15`; the
candidate SHA-256 was
`0be9540ea0a07a3ad17ca463a963b15479ae07773c796e91e8ec2621c45d5b52`. The
runner used fresh child processes and directories, alternated execution order,
and recorded wall time, user/system CPU time, maximum RSS, output bytes and
hashes, network counts, data rows, and command lines. Values below are median
baseline -> candidate seconds, followed by paired candidate-minus-baseline
percentage with inclusive IQR; negative means faster.

| Workload | Wall seconds; paired delta | CPU seconds; paired delta | Result |
| --- | --- | --- | --- |
| `blbr` | 0.024055 -> 0.024077; +0.847% [-1.657..+2.278] | 0.020343 -> 0.020497; +0.928% [-0.535..+2.404] | fallback/noise; reject as performance evidence |
| `SHP2_base_model` | 0.153589 -> 0.153425; -0.213% [-1.303..+0.721] | 0.147073 -> 0.146619; -0.173% [-1.015..+0.440] | neutral |
| `egfr_net` | 0.583170 -> 0.584392; +0.061% [-0.614..+1.293] | 0.565531 -> 0.563940; -0.144% [-0.574..+0.874] | neutral |
| `fceri_ji` | 0.678768 -> 0.656870; -3.736% [-4.714..-2.331] | 0.650026 -> 0.623834; -3.713% [-4.356..-2.403] | retain |

All 40 candidate runs produced byte-identical output-hash maps and sizes to
their paired baselines. Median maximum-RSS changes were -0.366%, -0.652%,
-0.277%, and +0.307% in workload order. The only material end-to-end gain is
the large FcERI ODE workload; the threshold gate avoids imposing packing cost
on the small fallback case. The independent Perl BNG2 generation checks still
show the pre-existing SHP2/blbr reaction-count mismatches, while EGFR and FcERI
match. A separate Perl-generated ODE trajectory could not be established
because the pinned reference environment lacks `run_network`; candidate-vs-
baseline hashes and the focused derivative test therefore provide the
available candidate evidence, not independent solver parity.

The baseline macOS `sample` profile used 30 FcERI model copies for 10 seconds.
The dominant work was CVODE's finite-difference Jacobian-vector path:
`OdeIntegrator::derivs` about 2,100 samples under `cvLsDQJtimes`, with
`SUNLinSolSolve_SPGMR` about 2,237 and `CVode` about 3,018. The final candidate
profile remained solver-dominated (`CVode` 3,062, `SUNLinSolSolve_SPGMR` 2,204,
`cvLsDQJtimes` 2,103, and `OdeIntegrator::derivs` 2,092); packing reduced the
reaction-update portion but did not remove the solver bottleneck.

Rejected ODE alternatives were measured and removed: skipping the fixed-
species scan was +0.514% wall / +0.535% CPU on FcERI, derivative-loop
specialization was +0.362% CPU on FcERI with cross-workload spread, a diagonal
preconditioner was +13.719% wall / +13.825% CPU and changed output hashes, and
an analytic mass-action J-times callback was +6.196% wall / +6.163% CPU and
changed output hashes. The compact representation is the remaining validated
portable CPU win. Release CTest passed 81/81 on the fresh build
`/private/tmp/bng-final-ode-20260901` (candidate executable SHA above). The
ASan/UBSan build passed its full 81-test run and the focused ODE test passed 23
assertions in 5 cases; all four production models exited successfully without
sanitizer diagnostics. The repository's full 41-model harness was rerun and
gave 34 passes and the same seven pre-existing environment/reference failures
listed earlier.

The remaining profile-dominant cost is CVODE's SPGMR/finite-difference J-times
interaction. A material next CPU gain requires a solver-aware Jacobian,
preconditioner, or reaction-network storage redesign; an equivalent GPU path
would require a broader device-resident integration design rather than a
per-callback offload. Those changes are outside this portable, semantics-
preserving branch.

## Canonical-certificate and deduplication redesign screens

The expanded canonical track tested three representation-level directions on
the existing `codex/canonical-redesign-20260901` source tip, then removed all
three because they were slower on the pinned production matrix. Each screen
used the retained canonical executable as baseline, 10 paired fresh processes
per model with alternating order, and deterministic output-hash comparison.
The baseline executable SHA-256 was
`9675de22ee67be6e2e5031ad815c6d033e9fe540ee07457043b334a19d533039`.

The benchmark commands had this common form, with the temporary candidate
executable varied per screen:

```sh
python3 Benchmarks/portable_cpu_benchmark.py \
  --executable-a /private/tmp/bng-final-canonical-20260901/src/bng_cpp \
  --executable-b /private/tmp/bng-canonical-certificate-screen \
  --model bng2/Models2/blbr.bngl \
  --model bng2/Models2/SHP2_base_model.bngl \
  --model bng2/Models2/egfr_net.bngl \
  --model bng2/Models2/fceri_ji.bngl \
  --repetitions 10 --timeout 30 \
  --output /private/tmp/portable_cpu_canonical_certificate_screen_10.json
```

The second screen used `bng-canonical-certificate-screen-fast` and
`portable_cpu_canonical_certificate_screen_fast_10.json`; the third used
`bng-canonical-refine-screen` and
`portable_cpu_canonical_refine_screen_10.json`. Candidate hashes were,
respectively, `4c920951aacdca8581f7e618405c39e9079c8a6b12a02d4d1b82eb5a53859a94`,
`288241c73050699cdbd7458091a171db3c2abc4d4942637b232152d7e1a94d69`, and
`c95fc2f13009ba1f826d9887171a93b8faba7b6ab0194dce86afa05ce092b40b`.
The table gives baseline-minus-candidate reduction, where positive is faster;
the bracket is inclusive IQR. Workloads are ordered `blbr`, SHP2, EGFR,
FcERI.

| Screen | Wall reduction by workload (median [IQR], %) | CPU-user reduction by workload (median [IQR], %) | Decision |
| --- | --- | --- | --- |
| Numeric certificate hash map plus sorted tokens | -4.342 [2.596], -1.097 [1.110], -1.464 [1.367], -1.067 [1.154] | -4.152 [2.387], -1.378 [1.351], -1.432 [1.619], -0.776 [1.074] | reject: slower across all four |
| Allocation-free commutative certificate accumulators | -2.582 [3.679], -1.251 [7.877], -1.138 [2.472], -2.010 [5.782] | -3.653 [2.254], -1.272 [4.107], -0.574 [2.128], -1.689 [4.237] | reject: slower across all four |
| Weisfeiler-Leman color refinement before Nauty (`nv >= 24`) | -5.113 [1.917], -2.108 [2.500], -4.415 [1.910], -1.521 [1.286] | -4.397 [2.524], -1.687 [2.395], -4.640 [1.706], -2.033 [1.639] | reject: added refinement cost |

All three screens produced byte-identical deterministic output maps to the
baseline for every paired run, but none produced a material end-to-end CPU
reduction. The first two screens added compact numeric invariants and
certificate indexes before the existing collision-safe canonical/Ullmann
path; the third added bounded color-refinement rounds before Nauty. Their
guard/index/refinement work costs more than it saves on these production
graphs. ASan/UBSan and Release validation for the retained source are recorded
above; no rejected implementation was committed. The remaining dominant
hotspot is therefore the canonical-certificate/deduplication representation
itself, where a material improvement requires a broader redesign of graph
storage, certificate reuse, collision handling, and ordering semantics.

## Separate opt-in parallel track

The existing `codex/parallel-optin-20260901` branch adds
`Benchmarks/parallel_batch.py`, an opt-in process-level launcher. Each model
runs in an isolated child process and working directory, so native mutable
graph state and generated files are not shared. The normal `bng_cpp` CLI and
single-process behavior are unchanged. The launcher records commands, model
and executable hashes, wall/CPU/RSS measurements, output sizes, and output
hashes.

The paired commands used the final launcher and executable hashes below. The
models are deterministic network/ODE workloads, so no stochastic seed is
needed for this matrix:

```sh
python3 /private/tmp/parallel_pair_benchmark.py \
  --batch-script Benchmarks/parallel_batch.py \
  --executable build/src/bng_cpp \
  --workload small=bng2/Models2/blbr.bngl,bng2/Models2/SHP2_base_model.bngl \
  --repetitions 20 --parallel-jobs 2 --timeout 60 \
  --output /private/tmp/portable_parallel_batch_small_medium_20.json

python3 /private/tmp/parallel_pair_benchmark.py \
  --batch-script Benchmarks/parallel_batch.py \
  --executable build/src/bng_cpp \
  --workload large=bng2/Models2/egfr_net.bngl,bng2/Models2/fceri_ji.bngl \
  --repetitions 20 --parallel-jobs 2 --timeout 60 \
  --output /private/tmp/portable_parallel_batch_large_20.json
```

The launcher SHA-256 was
`aeb709ff1d81675ee48372bc53f1471b296299284cbfc459820fe1c9675e5a39`; the
Release executable SHA-256 was
`c4eeb05df99869d34364671089df598c7ecfc5f700c70e957297e968cb9b4d15`.
Paired reductions are baseline-minus-candidate, with positive values faster:

| Batch | Wall median [IQR; min..max] | CPU user median [IQR; min..max] | RSS median [IQR; min..max] | Artifact equality |
| --- | ---: | ---: | ---: | --- |
| `blbr` + `SHP2_base_model` (20 pairs) | +13.614% [2.502; +7.061..+17.171] | -0.373% [3.034; -5.659..+4.401] | +0.410% [2.596; -2.852..+2.250] | 40/40 job maps equal |
| `egfr_net` + `fceri_ji` (20 pairs) | +45.808% [0.567; +41.810..+46.931] | -2.267% [1.045; -9.392..-0.754] | -0.177% [0.416; -3.633..+1.641] | 40/40 job maps equal |

The large-batch result is a material throughput win: all 20 pairs improved in
wall time while aggregate CPU user time remained approximately conserved and
all per-model `.net`, `.cdat`, and `.gdat` hashes matched. The small-batch
result is also wall-positive but CPU-neutral, so this is retained as an
opt-in batch-throughput facility, not as a claim that one model runs faster.
The limitation is fundamental to this implementation: independent models
can overlap, while a single model remains single-process and shared-state
safe. Further scaling requires a larger independent workload batch or a
broader thread-safe engine redesign.
