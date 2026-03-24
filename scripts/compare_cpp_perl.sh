#!/bin/bash
# Compare C++ and Perl BioNetGen network generation outputs
# Usage: ./compare_cpp_perl.sh <model.bngl> [output_dir]

set -e

if [ $# -lt 1 ]; then
    echo "Usage: $0 <model.bngl> [output_dir]"
    echo "Example: $0 tests/models/catalysis.bngl comparison_results"
    exit 1
fi

MODEL_FILE="$1"
OUTPUT_DIR="${2:-comparison_results}"
MODEL_NAME=$(basename "$MODEL_FILE" .bngl)

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "========================================="
echo "BioNetGen C++ vs Perl Comparison"
echo "========================================="
echo "Model: $MODEL_FILE"
echo "Output: $OUTPUT_DIR"
echo ""

# Create output directory
mkdir -p "$OUTPUT_DIR"

# Check if model file exists
if [ ! -f "$MODEL_FILE" ]; then
    echo -e "${RED}Error: Model file not found: $MODEL_FILE${NC}"
    exit 1
fi

# Check if C++ executable exists
CPP_BNG="./build/src/bng_cpp.exe"
if [ ! -f "$CPP_BNG" ]; then
    echo -e "${RED}Error: C++ executable not found: $CPP_BNG${NC}"
    echo "Run 'cmake --build build' to compile first."
    exit 1
fi

# Check if Perl BNG2 exists
PERL_BNG="./bng2/BNG2.pl"
if [ ! -f "$PERL_BNG" ]; then
    echo -e "${RED}Error: Perl BNG2.pl not found: $PERL_BNG${NC}"
    exit 1
fi

# Generate C++ network
echo -e "${YELLOW}[1/3] Generating C++ network...${NC}"
CPP_NET="$OUTPUT_DIR/${MODEL_NAME}_cpp.net"
MODEL_DIR=$(dirname "$MODEL_FILE")
if $CPP_BNG "$MODEL_FILE" > "$OUTPUT_DIR/${MODEL_NAME}_cpp.log" 2>&1; then
    # Move generated .net file (C++ writes it next to the source .bngl file)
    EXPECTED_NET="$MODEL_DIR/${MODEL_NAME}.net"
    if [ -f "$EXPECTED_NET" ]; then
        cp "$EXPECTED_NET" "$CPP_NET"
        rm "$EXPECTED_NET"
        echo -e "${GREEN}✓ C++ network generated: $CPP_NET${NC}"
    else
        echo -e "${RED}✗ C++ network generation failed (no .net file produced at $EXPECTED_NET)${NC}"
        cat "$OUTPUT_DIR/${MODEL_NAME}_cpp.log"
        exit 1
    fi
else
    echo -e "${RED}✗ C++ network generation failed${NC}"
    cat "$OUTPUT_DIR/${MODEL_NAME}_cpp.log"
    exit 1
fi

# Generate Perl network
echo -e "${YELLOW}[2/3] Generating Perl network...${NC}"
PERL_NET="$OUTPUT_DIR/${MODEL_NAME}_perl.net"
if perl "$PERL_BNG" "$MODEL_FILE" > "$OUTPUT_DIR/${MODEL_NAME}_perl.log" 2>&1; then
    # Move generated .net file (Perl writes it to current directory)
    if [ -f "${MODEL_NAME}.net" ]; then
        mv "${MODEL_NAME}.net" "$PERL_NET"
        echo -e "${GREEN}✓ Perl network generated: $PERL_NET${NC}"
    else
        echo -e "${RED}✗ Perl network generation failed (no .net file produced)${NC}"
        cat "$OUTPUT_DIR/${MODEL_NAME}_perl.log"
        exit 1
    fi
else
    echo -e "${RED}✗ Perl network generation failed${NC}"
    cat "$OUTPUT_DIR/${MODEL_NAME}_perl.log"
    exit 1
fi

echo ""
echo -e "${YELLOW}[3/3] Comparing outputs...${NC}"
echo ""

# Compare species
echo "--- Species Comparison ---"
if [ -f "bng2/Validate/compare_species.pl" ]; then
    perl bng2/Validate/compare_species.pl "$PERL_NET" "$CPP_NET" | tee "$OUTPUT_DIR/${MODEL_NAME}_species_diff.txt"
else
    echo -e "${YELLOW}Warning: compare_species.pl not found, skipping species comparison${NC}"
fi

echo ""
echo "--- Reaction Comparison ---"
if [ -f "bng2/Validate/compare_rxn.pl" ]; then
    perl bng2/Validate/compare_rxn.pl "$PERL_NET" "$CPP_NET" | tee "$OUTPUT_DIR/${MODEL_NAME}_rxn_diff.txt"
else
    echo -e "${YELLOW}Warning: compare_rxn.pl not found, skipping reaction comparison${NC}"
fi

echo ""
echo "--- Detailed Diff ---"
if [ -f "bng2/Perl2/diff_net.pl" ]; then
    perl bng2/Perl2/diff_net.pl "$PERL_NET" "$CPP_NET" | tee "$OUTPUT_DIR/${MODEL_NAME}_detailed_diff.txt"
else
    echo -e "${YELLOW}Warning: diff_net.pl not found, skipping detailed diff${NC}"
fi

echo ""
echo "========================================="
echo "Comparison complete!"
echo "Results saved to: $OUTPUT_DIR"
echo "========================================="
