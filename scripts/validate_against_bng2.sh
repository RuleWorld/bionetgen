#!/bin/bash
# Validation script: Compare C++ implementation against BNG2.pl reference outputs

echo "=== BNG2 Parity Validation for C++ Implementation ==="
echo ""

BNG_CPP="./build/src/bng_cpp.exe"
TEST_DIR="tests/models"
REF_DIR="tests/reference"

if [ ! -f "$BNG_CPP" ]; then
    echo "Error: bng_cpp.exe not found. Run cmake --build build first."
    exit 1
fi

PASSED=0
FAILED=0
TOTAL=0

for bngl_file in "$TEST_DIR"/*.bngl; do
    model=$(basename "$bngl_file" .bngl)
    ref_net="$REF_DIR/$model.net"

    # Skip if no reference file
    if [ ! -f "$ref_net" ]; then
        continue
    fi

    TOTAL=$((TOTAL + 1))
    echo -n "Testing $model... "

    # Run C++ implementation
    "$BNG_CPP" "$bngl_file" > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo "FAIL (execution error)"
        FAILED=$((FAILED + 1))
        continue
    fi

    # Compare network files
    generated_net="$TEST_DIR/$model.net"
    if [ ! -f "$generated_net" ]; then
        echo "FAIL (no .net generated)"
        FAILED=$((FAILED + 1))
        continue
    fi

    # Count species and reactions
    gen_species=$(awk '/begin species/,/end species/' "$generated_net" | grep -E "^[[:space:]]*[0-9]+" | wc -l)
    ref_species=$(awk '/begin species/,/end species/' "$ref_net" | grep -E "^[[:space:]]*[0-9]+" | wc -l)

    gen_rxns=$(awk '/begin reactions/,/end reactions/' "$generated_net" | grep -E "^[[:space:]]*[0-9]+" | wc -l)
    ref_rxns=$(awk '/begin reactions/,/end reactions/' "$ref_net" | grep -E "^[[:space:]]*[0-9]+" | wc -l)

    if [ "$gen_species" -eq "$ref_species" ] && [ "$gen_rxns" -eq "$ref_rxns" ]; then
        echo "PASS (species=$gen_species, reactions=$gen_rxns)"
        PASSED=$((PASSED + 1))
    else
        echo "FAIL (species: $gen_species vs $ref_species, reactions: $gen_rxns vs $ref_rxns)"
        FAILED=$((FAILED + 1))
    fi
done

echo ""
echo "=== Validation Summary ==="
echo "Total:  $TOTAL"
echo "Passed: $PASSED ($((PASSED * 100 / TOTAL))%)"
echo "Failed: $FAILED"
echo ""

if [ $FAILED -eq 0 ]; then
    echo "✓ All tests passed!"
    exit 0
else
    echo "✗ Some tests failed"
    exit 1
fi
