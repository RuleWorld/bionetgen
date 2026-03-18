#!/usr/bin/env bash
# Quick BioNetGen Performance Benchmark
# Single-run comparison between two git references
# Compatible with bash 3.2+ (macOS default)

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

# Default model
DEFAULT_MODEL="bng2/Models2/egfr_net.bngl"

# Colors
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

# Usage function
usage() {
    cat << EOF
Usage: $0 <baseline-ref> <comparison-ref> [model]

Arguments:
  baseline-ref      Git reference for baseline (branch, tag, or commit)
  comparison-ref    Git reference to compare against baseline
  model            Optional: model to test (default: egfr_net.bngl)

Examples:
  $0 v2.8.5 master
  $0 master develop fceri_ji
  $0 master feature-branch bng2/Models2/egfr_net.bngl

EOF
    exit 1
}

# Parse arguments
if [ $# -lt 2 ]; then
    usage
fi

BASELINE_REF="$1"
COMPARISON_REF="$2"
MODEL="${3:-$DEFAULT_MODEL}"

# Handle model path
if [[ ! "$MODEL" = /* ]] && [[ ! "$MODEL" = bng2/* ]]; then
    MODEL="bng2/Models2/$MODEL"
fi
if [[ ! "$MODEL" = *.bngl ]]; then
    MODEL="${MODEL}.bngl"
fi

echo "Quick Benchmark"
echo "=============================="
echo "Baseline:    $BASELINE_REF"
echo "Comparison:  $COMPARISON_REF"
echo "Model:       $(basename $MODEL)"
echo ""

# Verify git refs exist
if ! git rev-parse --verify "$BASELINE_REF" >/dev/null 2>&1; then
    echo -e "${RED}ERROR: Invalid baseline reference: $BASELINE_REF${NC}"
    exit 1
fi

if ! git rev-parse --verify "$COMPARISON_REF" >/dev/null 2>&1; then
    echo -e "${RED}ERROR: Invalid comparison reference: $COMPARISON_REF${NC}"
    exit 1
fi

# Verify model exists
if [ ! -f "$MODEL" ]; then
    echo -e "${RED}ERROR: Model not found: $MODEL${NC}"
    exit 1
fi

# Store original branch/commit
ORIGINAL_REF=$(git symbolic-ref -q HEAD 2>/dev/null || git rev-parse HEAD)

# Cleanup function
cleanup() {
    git checkout "$ORIGINAL_REF" --quiet 2>/dev/null || true
    git clean -fd bng2/Models2 > /dev/null 2>&1 || true
}
trap cleanup EXIT

# Check if rebuild needed
NEEDS_REBUILD=false
if git diff --name-only "$BASELINE_REF" "$COMPARISON_REF" | grep -q "bng2/Network3/.*\.\(cpp\|h\|hpp\|c\)$\|bng-graph/.*\.\(cpp\|h\|hpp\)$"; then
    NEEDS_REBUILD=true
    echo -e "${YELLOW}C++ changes detected - will rebuild binaries${NC}"
else
    echo -e "${BLUE}No C++ changes - using existing binaries${NC}"
fi
echo ""

# Function to rebuild
rebuild_network3() {
    echo -n "  Rebuilding Network3... "
    cd bng2/Network3
    make clean > /dev/null 2>&1 || true
    if make > /dev/null 2>&1; then
        cd "$REPO_ROOT"
        echo "done"
        return 0
    else
        cd "$REPO_ROOT"
        echo -e "${RED}FAILED${NC}"
        return 1
    fi
}

# Benchmark baseline
echo "Baseline ($BASELINE_REF):"
git checkout "$BASELINE_REF" --quiet 2>/dev/null
git clean -fd bng2/Models2 > /dev/null 2>&1 || true

if [ "$NEEDS_REBUILD" = true ]; then
    if ! rebuild_network3; then
        exit 1
    fi
fi

echo -n "  Running benchmark... "
START=$(perl -MTime::HiRes -e 'print Time::HiRes::time()')
perl bng2/BNG2.pl "$MODEL" > /dev/null 2>&1
END=$(perl -MTime::HiRes -e 'print Time::HiRes::time()')
BASELINE_TIME=$(perl -e "print $END - $START")
echo -e "${BLUE}${BASELINE_TIME}s${NC}"

# Benchmark comparison
echo ""
echo "Comparison ($COMPARISON_REF):"
git checkout "$COMPARISON_REF" --quiet 2>/dev/null
git clean -fd bng2/Models2 > /dev/null 2>&1 || true

if [ "$NEEDS_REBUILD" = true ]; then
    if ! rebuild_network3; then
        exit 1
    fi
fi

echo -n "  Running benchmark... "
START=$(perl -MTime::HiRes -e 'print Time::HiRes::time()')
perl bng2/BNG2.pl "$MODEL" > /dev/null 2>&1
END=$(perl -MTime::HiRes -e 'print Time::HiRes::time()')
COMPARISON_TIME=$(perl -e "print $END - $START")
echo -e "${BLUE}${COMPARISON_TIME}s${NC}"

# Calculate improvement
SPEEDUP=$(perl -e "print $BASELINE_TIME / $COMPARISON_TIME")
IMPROVEMENT=$(perl -e "print (($BASELINE_TIME - $COMPARISON_TIME) / $BASELINE_TIME) * 100")

echo ""
echo -e "${GREEN}Speedup: ${SPEEDUP}x (${IMPROVEMENT}% improvement)${NC}"
echo ""
echo "For more accurate results with multiple runs:"
echo "  cd Benchmarks && ./benchmark.sh $BASELINE_REF $COMPARISON_REF"
