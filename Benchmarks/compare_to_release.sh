#!/usr/bin/env bash
# Helper script to compare current master against latest release
# Useful for generating release notes

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

# Find latest release tag
LATEST_RELEASE=$(git tag | grep -E "^BioNetGen-[0-9]" | sort -V | tail -1)

if [ -z "$LATEST_RELEASE" ]; then
    echo "ERROR: Could not find any BioNetGen release tags"
    exit 1
fi

echo "Comparing master against latest release: $LATEST_RELEASE"
echo ""

# Allow override
BASELINE="${1:-$LATEST_RELEASE}"
COMPARISON="${2:-master}"

echo "Baseline:   $BASELINE"
echo "Comparison: $COMPARISON"
echo ""
echo "Press Enter to continue, Ctrl+C to cancel..."
read

# Run benchmark
cd "$SCRIPT_DIR"
./benchmark.sh "$BASELINE" "$COMPARISON" --runs 5

echo ""
echo "Results can be used for release notes or migration guide"
