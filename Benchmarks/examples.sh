#!/usr/bin/env bash
# Example benchmark commands for common use cases

# Compare current master against latest release
# (useful before creating a new release)
echo "Example 1: Compare master against last release tag"
echo "./benchmark.sh BioNetGen-2.8.5 master"
echo ""

# Compare a feature branch against develop
echo "Example 2: Compare feature branch against develop"
echo "./benchmark.sh develop feature/my-optimization"
echo ""

# Quick test with custom model
echo "Example 3: Quick test with specific model"
echo "./quick_benchmark.sh master develop fceri_ji"
echo ""

# Full benchmark with more runs for precision
echo "Example 4: More runs for precise measurements"
echo "./benchmark.sh master develop --runs 10"
echo ""

# Test only specific models
echo "Example 5: Test specific models only"
echo "./benchmark.sh master develop --models egfr_net,fceri_ji"
echo ""

# Compare against a specific commit
echo "Example 6: Compare against specific commit"
echo "./benchmark.sh abc1234 def5678"
echo ""

# For release notes - compare two release tags
echo "Example 7: Compare two releases (for migration guide)"
echo "./benchmark.sh BioNetGen-2.8.0 BioNetGen-2.8.5"
echo ""
