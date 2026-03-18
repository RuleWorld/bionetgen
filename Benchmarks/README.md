# BioNetGen Performance Benchmarks

This directory contains scripts for benchmarking BioNetGen performance across different versions, branches, or commits.

## Quick Start

### Compare two branches/commits:
```bash
./benchmark.sh master develop
```

### Compare against a release tag:
```bash
./benchmark.sh v2.8.0 master
```

### Quick single-run test:
```bash
./quick_benchmark.sh master develop
```

## Scripts

### `benchmark.sh`
Full benchmark suite with multiple runs and averaging.

**Usage:**
```bash
./benchmark.sh <baseline-ref> <comparison-ref> [options]
```

**Arguments:**
- `baseline-ref`: Git reference for baseline (branch, tag, or commit hash)
- `comparison-ref`: Git reference to compare against baseline

**Options:**
- `--runs N`: Number of runs per test (default: 3)
- `--models MODEL1,MODEL2,...`: Comma-separated list of models to test
- `--rebuild`: Force rebuild of Network3 binaries (default: auto-detect changes)

**Examples:**
```bash
# Compare current master against last release
./benchmark.sh BioNetGen-2.8.5 master

# Compare feature branch against develop
./benchmark.sh develop feature/new-algorithm

# Custom number of runs
./benchmark.sh master develop --runs 5

# Test specific models only
./benchmark.sh master develop --models egfr_net,fceri_ji
```

### `quick_benchmark.sh`
Fast single-run benchmark for quick testing.

**Usage:**
```bash
./quick_benchmark.sh <baseline-ref> <comparison-ref>
```

## Benchmark Models

Default models tested (from `bng2/Models2/`):
- `egfr_net.bngl` - EGFR signaling network (good for network generation performance)
- `fceri_ji.bngl` - FcεRI signaling (good for rule-based performance)

You can specify custom models with the `--models` option.

## Output

Results are saved to `results/` directory with timestamp:
```
results/
  benchmark_YYYYMMDD_HHMMSS.txt
  benchmark_YYYYMMDD_HHMMSS.csv
```

CSV format is suitable for importing into spreadsheets or plotting tools.

## Adding New Benchmark Models

To add a new model to the default benchmark suite:

1. Ensure the model file exists in `bng2/Models2/` or provide full path
2. Add to the `MODELS` array in `benchmark.sh`
3. Model should complete in reasonable time (< 2 minutes recommended)

## Notes

- Scripts automatically rebuild Network3 C++ binaries when switching branches
- **Optimized rebuild strategy**: Only 2 builds per benchmark (once per git ref), regardless of number of models
- Both Perl and C++ performance is measured
- Clean builds are performed for each test to ensure accuracy
- Original branch is restored after benchmarking

## Performance

The benchmark script is optimized to minimize rebuild overhead:

- **Old approach**: N models × 2 refs = 2N rebuilds
- **Current approach**: 2 rebuilds total (once per ref)

For 10 models, this reduces from 20 builds to just 2 builds, saving significant time!
