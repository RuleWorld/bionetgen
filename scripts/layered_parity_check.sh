#!/bin/bash
# Layered parity check for C++ BioNetGen against Perl reference .net files
# Inspired by the TypeScript web-simulator's layered_parity_check.ts
#
# Layers:
#   1. Parse: Can C++ parse the .bngl without error?
#   2. Generate: Does C++ produce a .net file?
#   3. Species count: Same number of species?
#   4. Reaction count: Same number of reactions?
#   5. Species match: Species strings match (via compare_species.pl)?
#   6. Reaction match: Reaction network matches (via compare_rxn.pl)?
#
# Usage: ./scripts/layered_parity_check.sh [model_name ...]

set -o pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
CPP_BNG="$ROOT_DIR/build/src/bng_cpp.exe"
VALIDATE_DIR="$ROOT_DIR/bng2/Validate"
DAT_DIR="$VALIDATE_DIR/DAT_validate"
COMPARE_SPECIES="$VALIDATE_DIR/compare_species.pl"
COMPARE_RXN="$VALIDATE_DIR/compare_rxn.pl"
TIMEOUT_SEC=60

if [ ! -f "$CPP_BNG" ]; then
    echo "ERROR: C++ executable not found. Run: cd build && cmake --build . --parallel"
    exit 1
fi

# Collect models
MODELS=()
if [ $# -gt 0 ]; then
    MODELS=("$@")
else
    for netfile in "$DAT_DIR"/*.net; do
        [ -f "$netfile" ] || continue
        model=$(basename "$netfile" .net)
        [ -f "$VALIDATE_DIR/$model.bngl" ] && MODELS+=("$model")
    done
fi

TMPDIR=$(mktemp -d)
trap "rm -rf $TMPDIR" EXIT

# Counters per layer
declare -A LAYER_PASS LAYER_FAIL
for layer in parse generate sp_count rx_count sp_match rx_match; do
    LAYER_PASS[$layer]=0
    LAYER_FAIL[$layer]=0
done
TOTAL_SKIP=0

count_lines() {
    local file="$1" section="$2"
    sed -n "/begin $section/,/end $section/p" "$file" 2>/dev/null | grep -c "^\s*[0-9]" 2>/dev/null || echo 0
}

echo "================================================================"
echo " Layered Parity Check: C++ vs Perl BNG2 Reference"
echo "================================================================"
echo ""
printf "%-35s %6s %6s %8s %8s %8s %8s\n" "Model" "Parse" "Gen" "Sp#" "Rx#" "SpStr" "RxStr"
printf "%-35s %6s %6s %8s %8s %8s %8s\n" "-----" "-----" "---" "---" "---" "-----" "-----"

for model in "${MODELS[@]}"; do
    BNGL="$VALIDATE_DIR/$model.bngl"
    REF_NET="$DAT_DIR/$model.net"

    if [ ! -f "$BNGL" ] || [ ! -f "$REF_NET" ]; then
        TOTAL_SKIP=$((TOTAL_SKIP + 1))
        continue
    fi

    WORKDIR="$TMPDIR/$model"
    mkdir -p "$WORKDIR"
    cp "$BNGL" "$WORKDIR/"
    [ -d "$VALIDATE_DIR/INPUT_FILES" ] && cp -r "$VALIDATE_DIR/INPUT_FILES" "$WORKDIR/" 2>/dev/null || true
    # Copy companion .net files (for models that use readFile)
    for netfile in "$VALIDATE_DIR"/*.net; do
        [ -f "$netfile" ] && cp "$netfile" "$WORKDIR/" 2>/dev/null || true
    done

    CPP_NET="$WORKDIR/$model.net"
    parse_ok="-"
    gen_ok="-"
    sp_count_ok="-"
    rx_count_ok="-"
    sp_match_ok="-"
    rx_match_ok="-"

    # Layer 1: Parse
    if timeout "$TIMEOUT_SEC" "$CPP_BNG" "$WORKDIR/$model.bngl" > "$WORKDIR/cpp.log" 2>&1; then
        parse_ok="PASS"
        LAYER_PASS[parse]=$((${LAYER_PASS[parse]} + 1))
    else
        # Check if it's a parse error vs runtime error
        if grep -q "parse failed\|syntax error\|mismatched input\|no viable alternative" "$WORKDIR/cpp.log" 2>/dev/null; then
            parse_ok="FAIL"
        elif grep -q "Killed\|timeout" "$WORKDIR/cpp.log" 2>/dev/null; then
            parse_ok="T/O"
        else
            parse_ok="ERR"
        fi
        LAYER_FAIL[parse]=$((${LAYER_FAIL[parse]} + 1))
        printf "%-35s %6s %6s %8s %8s %8s %8s\n" "$model" "$parse_ok" "-" "-" "-" "-" "-"
        continue
    fi

    # Layer 2: Generate
    if [ -f "$CPP_NET" ]; then
        gen_ok="PASS"
        LAYER_PASS[generate]=$((${LAYER_PASS[generate]} + 1))
    else
        gen_ok="FAIL"
        LAYER_FAIL[generate]=$((${LAYER_FAIL[generate]} + 1))
        printf "%-35s %6s %6s %8s %8s %8s %8s\n" "$model" "$parse_ok" "$gen_ok" "-" "-" "-" "-"
        continue
    fi

    # Layer 3: Species count
    cpp_sp=$(count_lines "$CPP_NET" "species")
    ref_sp=$(count_lines "$REF_NET" "species")
    if [ "$cpp_sp" = "$ref_sp" ]; then
        sp_count_ok="PASS"
        LAYER_PASS[sp_count]=$((${LAYER_PASS[sp_count]} + 1))
    else
        sp_count_ok="${cpp_sp}/${ref_sp}"
        LAYER_FAIL[sp_count]=$((${LAYER_FAIL[sp_count]} + 1))
    fi

    # Layer 4: Reaction count
    cpp_rx=$(count_lines "$CPP_NET" "reactions")
    ref_rx=$(count_lines "$REF_NET" "reactions")
    if [ "$cpp_rx" = "$ref_rx" ]; then
        rx_count_ok="PASS"
        LAYER_PASS[rx_count]=$((${LAYER_PASS[rx_count]} + 1))
    else
        rx_count_ok="${cpp_rx}/${ref_rx}"
        LAYER_FAIL[rx_count]=$((${LAYER_FAIL[rx_count]} + 1))
    fi

    # Layer 5: Species string match (only if count matches)
    if [ "$sp_count_ok" = "PASS" ] && [ -f "$COMPARE_SPECIES" ]; then
        if perl "$COMPARE_SPECIES" -n "$CPP_NET" "$REF_NET" > "$WORKDIR/sp_cmp.log" 2>&1; then
            sp_match_ok="PASS"
            LAYER_PASS[sp_match]=$((${LAYER_PASS[sp_match]} + 1))
        else
            sp_match_ok="FAIL"
            LAYER_FAIL[sp_match]=$((${LAYER_FAIL[sp_match]} + 1))
        fi
    fi

    # Layer 6: Reaction match (only if count matches)
    if [ "$rx_count_ok" = "PASS" ] && [ -f "$COMPARE_RXN" ]; then
        if perl "$COMPARE_RXN" "$CPP_NET" "$REF_NET" > "$WORKDIR/rx_cmp.log" 2>&1; then
            rx_match_ok="PASS"
            LAYER_PASS[rx_match]=$((${LAYER_PASS[rx_match]} + 1))
        else
            rx_match_ok="FAIL"
            LAYER_FAIL[rx_match]=$((${LAYER_FAIL[rx_match]} + 1))
        fi
    fi

    printf "%-35s %6s %6s %8s %8s %8s %8s\n" "$model" "$parse_ok" "$gen_ok" "$sp_count_ok" "$rx_count_ok" "$sp_match_ok" "$rx_match_ok"
done

echo ""
echo "================================================================"
echo " Summary"
echo "================================================================"
echo ""
printf "%-20s %6s %6s\n" "Layer" "Pass" "Fail"
printf "%-20s %6s %6s\n" "-----" "----" "----"
for layer in parse generate sp_count rx_count sp_match rx_match; do
    printf "%-20s %6d %6d\n" "$layer" "${LAYER_PASS[$layer]}" "${LAYER_FAIL[$layer]}"
done
echo ""
echo "Skipped: $TOTAL_SKIP"
echo ""

# Exit with total failures at Layer 3+ (count-level parity)
total_structural_fail=$(( ${LAYER_FAIL[sp_count]} + ${LAYER_FAIL[rx_count]} ))
if [ $total_structural_fail -eq 0 ] && [ ${LAYER_PASS[sp_count]} -gt 0 ]; then
    echo "All parsed models have correct species/reaction COUNTS."
fi
