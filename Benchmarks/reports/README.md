# Per-branch performance/validation reports

Each performance-optimization or validation write-up (native network
generation, ODE integration, canonical labeling, parallel batching, etc.)
gets its own file in this directory, named after the source branch, e.g.:

```
Benchmarks/reports/graph-string-20260901.md
Benchmarks/reports/ode-integration-20260901.md
Benchmarks/reports/canonical-redesign-20260901.md
```

**When writing up a new benchmark/validation result, create a new file
here — do not append to an existing report file, and do not append to
`Benchmarks/PORTABLE_CPU_PERFORMANCE.md`.** That file predates this
convention and accumulated conflicting appends from several branches
developed in parallel (PRs #329-#333), since every branch edited the same
tail of the same file. A new file per branch cannot collide with another
branch's new file, so this removes the recurring merge conflict entirely.

`Benchmarks/PORTABLE_CPU_PERFORMANCE.md` is kept as-is as the historical
record of that series and is not being retroactively split.

Each report should be self-contained: state the branch name and base
commit, the mechanism changed, the exact benchmark command(s) run (see
`Benchmarks/portable_cpu_benchmark.py --help`), and the correctness
evidence (test results, independent-reference comparisons) alongside the
performance numbers.
