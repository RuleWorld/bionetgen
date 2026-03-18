# Benchmark Framework Changelog

## Recent Optimizations

### Rebuild Optimization (2026-03-18)

**Problem**: The original benchmark script rebuilt binaries for each model, resulting in redundant compilations.

**Before**:
```
For each model:
  - Switch to baseline ref
  - Rebuild binaries (Build #1, #3, #5...)
  - Run model
  - Switch to comparison ref
  - Rebuild binaries (Build #2, #4, #6...)
  - Run model
```

Result: With N models, we performed **2×N rebuilds**.

**After**:
```
Switch to baseline ref
Rebuild binaries (Build #1)
For each model:
  - Run model

Switch to comparison ref
Rebuild binaries (Build #2)
For each model:
  - Run model
```

Result: **2 rebuilds total**, regardless of number of models.

**Performance Impact**:
- 2 models: 4 builds → 2 builds (50% reduction)
- 5 models: 10 builds → 2 builds (80% reduction)
- 10 models: 20 builds → 2 builds (90% reduction)

**Time Savings** (assuming ~30s per build):
- 2 models: ~60s saved
- 5 models: ~4min saved
- 10 models: ~9min saved

### Future Optimization Ideas

#### Binary Caching
Cache compiled binaries by git commit hash to avoid rebuilding the same version:

```bash
# Pseudocode
if [ -f "cache/bin_${COMMIT_HASH}.tar.gz" ]; then
    extract_cached_binary
else
    rebuild_and_cache
fi
```

This would be especially useful when:
- Running benchmarks against the same release tag multiple times
- Comparing multiple branches against the same baseline
- Re-running benchmarks after fixing test issues

**Potential Implementation**:
- Cache directory: `Benchmarks/.cache/binaries/`
- Key: `${commit_hash}_${platform}` (e.g., `38909a79_darwin`)
- Add `--no-cache` flag to force rebuild
- Auto-cleanup cache older than 30 days

**Additional Savings**:
If you benchmark 5 different feature branches against the same baseline (e.g., `master`), you'd save 5 rebuilds of master (assuming binary caching is implemented).

## Version History

### v1.1 (2026-03-18)
- Optimized rebuild strategy (loop inversion)
- Reduced builds from 2N to 2

### v1.0 (2026-03-18)
- Initial benchmark framework
- Support for arbitrary git references
- Multiple output formats (TXT, CSV)
- Statistical analysis with mean and std dev
- Smart C++ change detection
