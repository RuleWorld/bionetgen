#!/bin/bash
# Validate C++ BioNetGen against Perl reference .net files
# Uses the existing Validate/DAT_validate/ reference files and comparison scripts
#
# Usage: ./scripts/validate_cpp_against_perl.sh [model_name]
#   Without arguments: runs all models with .net references
#   With argument: runs only the specified model

set -o pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
CPP_BNG="$ROOT_DIR/build/src/bng_cpp.exe"
VALIDATE_DIR="$ROOT_DIR/bng2/Validate"
DAT_DIR="$VALIDATE_DIR/DAT_validate"
COMPARE_SPECIES="$VALIDATE_DIR/compare_species.pl"
COMPARE_RXN="$VALIDATE_DIR/compare_rxn.pl"

# Check prerequisites
if [ ! -f "$CPP_BNG" ]; then
    echo "ERROR: C++ executable not found at $CPP_BNG"
    echo "Run: cd build && cmake --build . --parallel"
    exit 1
fi

# Collect models to test
MODELS=()
if [ $# -gt 0 ]; then
    MODELS=("$@")
else
    for netfile in "$DAT_DIR"/*.net; do
        [ -f "$netfile" ] || continue
        model=$(basename "$netfile" .net)
        # Only test if corresponding .bngl exists
        if [ -f "$VALIDATE_DIR/$model.bngl" ]; then
            MODELS+=("$model")
        fi
    done
fi

PASS=0
FAIL=0
SKIP=0
ERRORS=()

TMPDIR=$(mktemp -d)
trap "rm -rf $TMPDIR" EXIT

echo "==========================================="
echo " C++ BioNetGen Validation Against Perl"
echo "==========================================="
echo "Models to test: ${#MODELS[@]}"
echo ""

for model in "${MODELS[@]}"; do
    BNGL="$VALIDATE_DIR/$model.bngl"
    REF_NET="$DAT_DIR/$model.net"

    if [ ! -f "$BNGL" ]; then
        echo "SKIP  $model  (no .bngl file)"
        SKIP=$((SKIP + 1))
        continue
    fi

    if [ ! -f "$REF_NET" ]; then
        echo "SKIP  $model  (no reference .net)"
        SKIP=$((SKIP + 1))
        continue
    fi

    # Create working directory for this model
    WORKDIR="$TMPDIR/$model"
    mkdir -p "$WORKDIR"
    cp "$BNGL" "$WORKDIR/"

    # Copy any auxiliary input files the model might need
    if [ -d "$VALIDATE_DIR/INPUT_FILES" ]; then
        cp -r "$VALIDATE_DIR/INPUT_FILES" "$WORKDIR/" 2>/dev/null || true
    fi

    # Run C++ network generation (30s timeout)
    CPP_NET="$WORKDIR/$model.net"
    if ! timeout 30 "$CPP_BNG" "$WORKDIR/$model.bngl" > "$WORKDIR/cpp.log" 2>&1; then
        echo "FAIL  $model  (C++ crashed or errored)"
        cat "$WORKDIR/cpp.log" | tail -3 | sed 's/^/      /'
        FAIL=$((FAIL + 1))
        ERRORS+=("$model: C++ execution failed")
        continue
    fi

    if [ ! -f "$CPP_NET" ]; then
        echo "FAIL  $model  (no .net file produced)"
        FAIL=$((FAIL + 1))
        ERRORS+=("$model: no .net output")
        continue
    fi

    # Compare species
    species_ok=true
    if [ -f "$COMPARE_SPECIES" ]; then
        species_out=$(perl "$COMPARE_SPECIES" -n "$CPP_NET" "$REF_NET" 2>&1)
        species_exit=$?
        if [ $species_exit -ne 0 ]; then
            species_ok=false
        fi
    fi

    # Compare reactions
    rxn_ok=true
    if [ -f "$COMPARE_RXN" ]; then
        rxn_out=$(perl "$COMPARE_RXN" "$CPP_NET" "$REF_NET" 2>&1)
        rxn_exit=$?
        if [ $rxn_exit -ne 0 ]; then
            rxn_ok=false
        fi
    fi

    # Report result
    if $species_ok && $rxn_ok; then
        # Also compare counts as sanity check
        cpp_sp=$(grep -c "^\s*[0-9]" <(sed -n '/begin species/,/end species/p' "$CPP_NET") 2>/dev/null || echo 0)
        ref_sp=$(grep -c "^\s*[0-9]" <(sed -n '/begin species/,/end species/p' "$REF_NET") 2>/dev/null || echo 0)
        cpp_rx=$(grep -c "^\s*[0-9]" <(sed -n '/begin reactions/,/end reactions/p' "$CPP_NET") 2>/dev/null || echo 0)
        ref_rx=$(grep -c "^\s*[0-9]" <(sed -n '/begin reactions/,/end reactions/p' "$REF_NET") 2>/dev/null || echo 0)
        echo "PASS  $model  (species=$cpp_sp/$ref_sp  reactions=$cpp_rx/$ref_rx)"
        PASS=$((PASS + 1))
    else
        cpp_sp=$(grep -c "^\s*[0-9]" <(sed -n '/begin species/,/end species/p' "$CPP_NET") 2>/dev/null || echo 0)
        ref_sp=$(grep -c "^\s*[0-9]" <(sed -n '/begin species/,/end species/p' "$REF_NET") 2>/dev/null || echo 0)
        cpp_rx=$(grep -c "^\s*[0-9]" <(sed -n '/begin reactions/,/end reactions/p' "$CPP_NET") 2>/dev/null || echo 0)
        ref_rx=$(grep -c "^\s*[0-9]" <(sed -n '/begin reactions/,/end reactions/p' "$REF_NET") 2>/dev/null || echo 0)
        echo "FAIL  $model  (species=$cpp_sp/$ref_sp  reactions=$cpp_rx/$ref_rx)"
        if ! $species_ok; then
            echo "$species_out" | grep -E "Mismatch|Incompatible|No match" | head -3 | sed 's/^/      /'
        fi
        if ! $rxn_ok; then
            echo "$rxn_out" | grep -E "Mismatch|Incompatible|No match" | head -3 | sed 's/^/      /'
        fi
        FAIL=$((FAIL + 1))
        ERRORS+=("$model: species_ok=$species_ok rxn_ok=$rxn_ok (sp=$cpp_sp/$ref_sp rx=$cpp_rx/$ref_rx)")
    fi
done

echo ""
echo "==========================================="
echo " Results: $PASS passed, $FAIL failed, $SKIP skipped"
echo "==========================================="

if [ ${#ERRORS[@]} -gt 0 ]; then
    echo ""
    echo "Failures:"
    for err in "${ERRORS[@]}"; do
        echo "  - $err"
    done
fi

exit $FAIL
