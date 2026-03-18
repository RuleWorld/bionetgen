# Benchmark Usage Guide

## Quick Start

### 1. Compare current master to latest release
```bash
cd Benchmarks
./compare_to_release.sh
```

This automatically finds the latest release tag and runs a full benchmark.

### 2. Quick single-run test
```bash
cd Benchmarks
./quick_benchmark.sh BioNetGen-2.9.3 master
```

### 3. Full benchmark with custom options
```bash
cd Benchmarks
./benchmark.sh BioNetGen-2.9.3 master --runs 5
```

## Common Use Cases

### Before a New Release

Compare master against the previous release to document performance improvements:

```bash
cd Benchmarks
./compare_to_release.sh
```

This will:
- Find the latest release tag (e.g., `BioNetGen-2.9.3`)
- Compare it against current `master`
- Run 5 iterations for statistical confidence
- Save results to `results/benchmark_YYYYMMDD_HHMMSS.{txt,csv}`

Use the results in your release notes!

### Testing a Feature Branch

Before merging a performance-related feature:

```bash
cd Benchmarks
./benchmark.sh master feature/my-optimization --runs 3
```

### Comparing Two Releases

For migration guides or historical analysis:

```bash
cd Benchmarks
./benchmark.sh BioNetGen-2.8.5 BioNetGen-2.9.3
```

## Understanding Results

### Output Format

```
Model                Baseline (s)   Comparison (s)     Speedup  Improvement
------------------------------------------------------------------------------------
egfr_net                9.50 (±0.12)      6.10 (±0.08)      1.56x        35.8%
fceri_ji                4.10 (±0.05)      2.40 (±0.03)      1.71x        41.5%
```

- **Baseline**: Time for the reference version (with standard deviation)
- **Comparison**: Time for the new version (with standard deviation)
- **Speedup**: How many times faster (>1.0 means improvement)
- **Improvement**: Percentage faster (positive means improvement)

### Statistical Confidence

- Use `--runs 3` (default) for quick checks
- Use `--runs 5` or higher for release benchmarks
- Standard deviation shows measurement consistency
- Lower std dev = more reliable measurement

## Advanced Options

### Custom Models

Test only specific models:

```bash
./benchmark.sh master develop --models egfr_net,fceri_ji
```

Add your own model (must be in `bng2/Models2/`):

```bash
./benchmark.sh master develop --models my_model
```

### Controlling Rebuilds

The scripts automatically detect C++ changes and rebuild when needed. Override:

```bash
# Force rebuild even if no C++ changes
./benchmark.sh master develop --rebuild

# Skip rebuild (Perl-only changes)
./benchmark.sh master develop --no-rebuild
```

## Results Files

Results are saved in two formats:

1. **TXT** (`results/benchmark_YYYYMMDD_HHMMSS.txt`):
   - Human-readable
   - Includes git commit hashes
   - Good for release notes

2. **CSV** (`results/benchmark_YYYYMMDD_HHMMSS.csv`):
   - Machine-readable
   - Import into Excel/Google Sheets
   - Plot performance trends over time

## Tips

### For Release Notes

1. Run benchmark against previous release:
   ```bash
   ./compare_to_release.sh
   ```

2. Use at least 5 runs for reliability:
   ```bash
   ./benchmark.sh BioNetGen-2.9.3 master --runs 5
   ```

3. Copy results from `results/*.txt` into release notes

### For Performance Debugging

1. Start with quick benchmark:
   ```bash
   ./quick_benchmark.sh master feature-branch
   ```

2. If you see improvement, validate with full benchmark:
   ```bash
   ./benchmark.sh master feature-branch --runs 5
   ```

3. Test with relevant models:
   ```bash
   ./benchmark.sh master feature-branch --models model_that_uses_my_feature
   ```

### Building a Performance History

Save benchmark results over time:

```bash
# Every release
./benchmark.sh BioNetGen-2.9.2 BioNetGen-2.9.3
./benchmark.sh BioNetGen-2.9.3 BioNetGen-2.9.4
# etc.
```

Use CSV files to plot performance trends across releases.

## Troubleshooting

### "Model not found" error

Model paths are resolved relative to `bng2/Models2/`. Use:
```bash
--models egfr_net          # Will use bng2/Models2/egfr_net.bngl
--models bng2/Models2/custom/my_model.bngl  # Use full path
```

### "Failed to build Network3" error

Check that the selected git refs have valid C++ code:
```bash
git checkout <ref>
cd bng2/Network3
make
```

### Inconsistent Results (high std dev)

- Close other applications
- Run more iterations: `--runs 10`
- Check if system is under load
- Some models have inherent variability (SSA methods)
