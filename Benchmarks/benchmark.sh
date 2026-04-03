#!/usr/bin/env bash
# BioNetGen Performance Benchmark Suite
# Compares performance between two git references (branches, tags, commits)
# Compatible with bash 3.2+ (macOS default)

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

# Default configuration
DEFAULT_MODELS=(
    "bng2/Models2/egfr_net.bngl"
    "bng2/Models2/fceri_ji.bngl"
)
RUNS=3
REBUILD_NETWORK3=auto

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Usage function
usage() {
    cat << EOF
Usage: $0 <baseline-ref> <comparison-ref> [options]

Arguments:
  baseline-ref      Git reference for baseline (branch, tag, or commit)
  comparison-ref    Git reference to compare against baseline

Options:
  --runs N          Number of runs per test (default: 3)
  --models M1,M2    Comma-separated model list (default: egfr_net,fceri_ji)
  --rebuild         Force rebuild of Network3 binaries
  --no-rebuild      Skip rebuilding binaries
  -h, --help        Show this help message

Examples:
  $0 v2.8.5 master
  $0 master develop --runs 5
  $0 master feature-branch --models egfr_net

EOF
    exit 1
}

# Parse arguments
if [ $# -lt 2 ]; then
    usage
fi

BASELINE_REF="$1"
COMPARISON_REF="$2"
shift 2

MODELS=("${DEFAULT_MODELS[@]}")

while [[ $# -gt 0 ]]; do
    case $1 in
        --runs)
            RUNS="$2"
            shift 2
            ;;
        --models)
            IFS=',' read -ra MODELS <<< "$2"
            # Convert relative paths to full paths
            for i in "${!MODELS[@]}"; do
                if [[ ! "${MODELS[$i]}" = /* ]] && [[ ! "${MODELS[$i]}" = bng2/* ]]; then
                    MODELS[$i]="bng2/Models2/${MODELS[$i]}"
                fi
                # Add .bngl extension if missing
                if [[ ! "${MODELS[$i]}" = *.bngl ]]; then
                    MODELS[$i]="${MODELS[$i]}.bngl"
                fi
            done
            shift 2
            ;;
        --rebuild)
            REBUILD_NETWORK3=always
            shift
            ;;
        --no-rebuild)
            REBUILD_NETWORK3=never
            shift
            ;;
        -h|--help)
            usage
            ;;
        *)
            echo "Unknown option: $1"
            usage
            ;;
    esac
done

# Create results directory
RESULTS_DIR="$SCRIPT_DIR/results"
mkdir -p "$RESULTS_DIR"

# Generate timestamp for output files
TIMESTAMP=$(date +%Y%m%d_%H%M%S)
RESULTS_TXT="$RESULTS_DIR/benchmark_${TIMESTAMP}.txt"
RESULTS_CSV="$RESULTS_DIR/benchmark_${TIMESTAMP}.csv"

echo "=========================================="
echo "BioNetGen Performance Benchmark"
echo "=========================================="
echo ""
echo "Baseline:    $BASELINE_REF"
echo "Comparison:  $COMPARISON_REF"
echo "Runs:        $RUNS"
echo "Models:      ${#MODELS[@]}"
echo ""
echo "Results will be saved to:"
echo "  $RESULTS_TXT"
echo "  $RESULTS_CSV"
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

# Verify models exist
for model in "${MODELS[@]}"; do
    if [ ! -f "$model" ]; then
        echo -e "${RED}ERROR: Model not found: $model${NC}"
        exit 1
    fi
done

# Store original branch/commit
ORIGINAL_REF=$(git symbolic-ref -q HEAD 2>/dev/null || git rev-parse HEAD)

# Cleanup function
cleanup() {
    echo ""
    echo "Cleaning up..."
    git checkout "$ORIGINAL_REF" --quiet 2>/dev/null || true
    git clean -fd bng2/Models2 > /dev/null 2>&1 || true
}
trap cleanup EXIT

# Function to check if Network3 needs rebuild
needs_rebuild() {
    local ref1="$1"
    local ref2="$2"

    if [ "$REBUILD_NETWORK3" = "always" ]; then
        return 0
    elif [ "$REBUILD_NETWORK3" = "never" ]; then
        return 1
    fi

    # Check if any C++ files changed between refs
    if git diff --name-only "$ref1" "$ref2" | grep -q "bng2/Network3/.*\.\(cpp\|h\|hpp\|c\)$\|bng-graph/.*\.\(cpp\|h\|hpp\)$"; then
        return 0
    else
        return 1
    fi
}

# Function to rebuild Network3 binaries
rebuild_network3() {
    echo -n "    Rebuilding Network3 C++ binaries... "
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

# Function to run a single benchmark
run_benchmark() {
    local model="$1"
    local log_file="$2"

    # Run BNG2.pl and capture output to log
    local start=$(perl -MTime::HiRes -e 'print Time::HiRes::time()')
    perl bng2/BNG2.pl "$model" > "$log_file" 2>&1
    local end=$(perl -MTime::HiRes -e 'print Time::HiRes::time()')

    # Calculate wall-clock time
    local wallclock=$(perl -e "print $end - $start")

    # Parse CPU times from log
    local generate_time=$(grep "CPU TIME: generate_network" "$log_file" | awk '{print $4}' || echo "0")
    local simulate_time=$(grep "CPU TIME: simulate" "$log_file" | awk '{print $4}' || echo "0")
    local total_cpu_time=$(grep "CPU TIME: total" "$log_file" | awk '{print $4}' || echo "0")

    # Return comma-separated: wallclock,generate,simulate,total_cpu
    echo "$wallclock,$generate_time,$simulate_time,$total_cpu_time"
}

# Function to calculate average
calc_average() {
    local sum=0
    local count=0
    for time in "$@"; do
        sum=$(perl -e "print $sum + $time")
        count=$((count + 1))
    done
    perl -e "print $sum / $count"
}

# Function to calculate standard deviation
calc_stddev() {
    local mean="$1"
    shift
    local sum_sq=0
    local count=0

    for time in "$@"; do
        sum_sq=$(perl -e "print $sum_sq + ($time - $mean) ** 2")
        count=$((count + 1))
    done

    perl -e "print sqrt($sum_sq / $count)"
}

# Check if rebuild needed
NEEDS_REBUILD=false
if needs_rebuild "$BASELINE_REF" "$COMPARISON_REF"; then
    NEEDS_REBUILD=true
    echo -e "${YELLOW}C++ code changes detected - will rebuild binaries${NC}"

    # Show some specific changes for verification
    echo ""
    echo "Sample of changed files:"
    git diff --name-only "$BASELINE_REF" "$COMPARISON_REF" | grep -E "\.(cpp|h|hpp|pm)$" | head -5 | sed 's/^/  - /'
    echo ""
else
    echo -e "${BLUE}No C++ changes detected - using existing binaries${NC}"
fi
echo ""

# Results storage (bash 3.2 compatible)
results_names=()
results_baseline_all=()
results_comparison_all=()

# Create temp directory for logs
TEMP_LOG_DIR=$(mktemp -d "${TMPDIR:-/tmp}/bng_benchmark.XXXXXX")
trap 'rm -rf "$TEMP_LOG_DIR"' EXIT

echo -e "${BLUE}Running benchmarks...${NC}"
echo ""

# Build model names array
for model in "${MODELS[@]}"; do
    model_name=$(basename "$model" .bngl)
    results_names+=("$model_name")
done

# ========================================
# BENCHMARK BASELINE REF
# ========================================
echo -e "${YELLOW}=== Benchmarking Baseline: $BASELINE_REF ===${NC}"
git checkout "$BASELINE_REF" --quiet 2>/dev/null
git clean -fd bng2/Models2 > /dev/null 2>&1 || true

# Verify we're on the correct ref
CURRENT_COMMIT=$(git rev-parse --short HEAD)
echo "  Git commit: $CURRENT_COMMIT"

if [ "$NEEDS_REBUILD" = true ]; then
    if ! rebuild_network3; then
        echo -e "${RED}ERROR: Failed to build Network3 on $BASELINE_REF${NC}"
        exit 1
    fi
    # Show binary timestamp to verify it was rebuilt
    echo "  Binary built: $(date -r bng2/bin/run_network '+%Y-%m-%d %H:%M:%S' 2>/dev/null || stat -f '%Sm' -t '%Y-%m-%d %H:%M:%S' bng2/bin/run_network 2>/dev/null || echo 'timestamp unavailable')"
fi

for model in "${MODELS[@]}"; do
    model_name=$(basename "$model" .bngl)
    echo -n "  $model_name: "

    # Arrays for each timing phase
    wallclock_times=()
    generate_times=()
    simulate_times=()
    total_cpu_times=()

    for run in $(seq 1 $RUNS); do
        log_file="$TEMP_LOG_DIR/baseline_${model_name}_run${run}.log"
        result=$(run_benchmark "$model" "$log_file")

        # Parse result: wallclock,generate,simulate,total_cpu
        IFS=',' read wc gen sim cpu <<< "$result"
        wallclock_times+=($wc)
        generate_times+=($gen)
        simulate_times+=($sim)
        total_cpu_times+=($cpu)
        echo -n "."
    done

    # Calculate averages and stddevs for each phase
    avg_wc=$(calc_average "${wallclock_times[@]}")
    std_wc=$(calc_stddev "$avg_wc" "${wallclock_times[@]}")
    avg_gen=$(calc_average "${generate_times[@]}")
    std_gen=$(calc_stddev "$avg_gen" "${generate_times[@]}")
    avg_sim=$(calc_average "${simulate_times[@]}")
    std_sim=$(calc_stddev "$avg_sim" "${simulate_times[@]}")
    avg_cpu=$(calc_average "${total_cpu_times[@]}")
    std_cpu=$(calc_stddev "$avg_cpu" "${total_cpu_times[@]}")

    echo " wall=${avg_wc}s gen=${avg_gen}s sim=${avg_sim}s"

    # Store all times as comma-separated string: wall,wall_std,gen,gen_std,sim,sim_std,cpu,cpu_std
    results_baseline_all+=("$avg_wc,$std_wc,$avg_gen,$std_gen,$avg_sim,$std_sim,$avg_cpu,$std_cpu")
done

echo ""

# ========================================
# BENCHMARK COMPARISON REF
# ========================================
echo -e "${YELLOW}=== Benchmarking Comparison: $COMPARISON_REF ===${NC}"
git checkout "$COMPARISON_REF" --quiet 2>/dev/null
git clean -fd bng2/Models2 > /dev/null 2>&1 || true

# Verify we're on the correct ref
CURRENT_COMMIT=$(git rev-parse --short HEAD)
echo "  Git commit: $CURRENT_COMMIT"

if [ "$NEEDS_REBUILD" = true ]; then
    if ! rebuild_network3; then
        echo -e "${RED}ERROR: Failed to build Network3 on $COMPARISON_REF${NC}"
        exit 1
    fi
    # Show binary timestamp to verify it was rebuilt
    echo "  Binary built: $(date -r bng2/bin/run_network '+%Y-%m-%d %H:%M:%S' 2>/dev/null || stat -f '%Sm' -t '%Y-%m-%d %H:%M:%S' bng2/bin/run_network 2>/dev/null || echo 'timestamp unavailable')"
fi

for model in "${MODELS[@]}"; do
    model_name=$(basename "$model" .bngl)
    echo -n "  $model_name: "

    # Arrays for each timing phase
    wallclock_times=()
    generate_times=()
    simulate_times=()
    total_cpu_times=()

    for run in $(seq 1 $RUNS); do
        log_file="$TEMP_LOG_DIR/comparison_${model_name}_run${run}.log"
        result=$(run_benchmark "$model" "$log_file")

        # Parse result: wallclock,generate,simulate,total_cpu
        IFS=',' read wc gen sim cpu <<< "$result"
        wallclock_times+=($wc)
        generate_times+=($gen)
        simulate_times+=($sim)
        total_cpu_times+=($cpu)
        echo -n "."
    done

    # Calculate averages and stddevs for each phase
    avg_wc=$(calc_average "${wallclock_times[@]}")
    std_wc=$(calc_stddev "$avg_wc" "${wallclock_times[@]}")
    avg_gen=$(calc_average "${generate_times[@]}")
    std_gen=$(calc_stddev "$avg_gen" "${generate_times[@]}")
    avg_sim=$(calc_average "${simulate_times[@]}")
    std_sim=$(calc_stddev "$avg_sim" "${simulate_times[@]}")
    avg_cpu=$(calc_average "${total_cpu_times[@]}")
    std_cpu=$(calc_stddev "$avg_cpu" "${total_cpu_times[@]}")

    echo " wall=${avg_wc}s gen=${avg_gen}s sim=${avg_sim}s"

    # Store all times as comma-separated string: wall,wall_std,gen,gen_std,sim,sim_std,cpu,cpu_std
    results_comparison_all+=("$avg_wc,$std_wc,$avg_gen,$std_gen,$avg_sim,$std_sim,$avg_cpu,$std_cpu")
done

echo ""

# Generate results table
echo ""
echo "=========================================="
echo "RESULTS SUMMARY"
echo "=========================================="
echo ""
echo "Overall Performance:"
printf "%-20s %15s %15s %12s %12s\n" "Model" "Baseline (s)" "Comparison (s)" "Speedup" "Improvement"
echo "------------------------------------------------------------------------------------"

for i in "${!results_names[@]}"; do
    model_name="${results_names[$i]}"

    # Parse baseline results: wall,wall_std,gen,gen_std,sim,sim_std,cpu,cpu_std
    IFS=',' read b_wall b_wall_std b_gen b_gen_std b_sim b_sim_std b_cpu b_cpu_std <<< "${results_baseline_all[$i]}"

    # Parse comparison results
    IFS=',' read c_wall c_wall_std c_gen c_gen_std c_sim c_sim_std c_cpu c_cpu_std <<< "${results_comparison_all[$i]}"

    speedup=$(perl -e "print $b_wall / $c_wall")
    improvement=$(perl -e "print (($b_wall - $c_wall) / $b_wall) * 100")

    printf "%-20s %7.2f (±%.2f) %7.2f (±%.2f) %11.2fx %10.1f%%\n" \
        "$model_name" "$b_wall" "$b_wall_std" "$c_wall" "$c_wall_std" "$speedup" "$improvement"
done

echo ""
echo "Phase Breakdown - generate_network():"
printf "%-20s %15s %15s %12s %12s\n" "Model" "Baseline (s)" "Comparison (s)" "Speedup" "Improvement"
echo "------------------------------------------------------------------------------------"

for i in "${!results_names[@]}"; do
    model_name="${results_names[$i]}"

    IFS=',' read b_wall b_wall_std b_gen b_gen_std b_sim b_sim_std b_cpu b_cpu_std <<< "${results_baseline_all[$i]}"
    IFS=',' read c_wall c_wall_std c_gen c_gen_std c_sim c_sim_std c_cpu c_cpu_std <<< "${results_comparison_all[$i]}"

    if [ "$(echo "$b_gen > 0" | bc 2>/dev/null || perl -e "print $b_gen > 0 ? 1 : 0")" = "1" ]; then
        gen_speedup=$(perl -e "print $b_gen / $c_gen")
        gen_improvement=$(perl -e "print (($b_gen - $c_gen) / $b_gen) * 100")
        printf "%-20s %7.2f (±%.2f) %7.2f (±%.2f) %11.2fx %10.1f%%\n" \
            "$model_name" "$b_gen" "$b_gen_std" "$c_gen" "$c_gen_std" "$gen_speedup" "$gen_improvement"
    else
        printf "%-20s %15s %15s %12s %12s\n" "$model_name" "N/A" "N/A" "-" "-"
    fi
done

echo ""
echo "Phase Breakdown - simulate():"
printf "%-20s %15s %15s %12s %12s\n" "Model" "Baseline (s)" "Comparison (s)" "Speedup" "Improvement"
echo "------------------------------------------------------------------------------------"

for i in "${!results_names[@]}"; do
    model_name="${results_names[$i]}"

    IFS=',' read b_wall b_wall_std b_gen b_gen_std b_sim b_sim_std b_cpu b_cpu_std <<< "${results_baseline_all[$i]}"
    IFS=',' read c_wall c_wall_std c_gen c_gen_std c_sim c_sim_std c_cpu c_cpu_std <<< "${results_comparison_all[$i]}"

    if [ "$(echo "$b_sim > 0" | bc 2>/dev/null || perl -e "print $b_sim > 0 ? 1 : 0")" = "1" ]; then
        sim_speedup=$(perl -e "print $b_sim / $c_sim")
        sim_improvement=$(perl -e "print (($b_sim - $c_sim) / $b_sim) * 100")
        printf "%-20s %7.2f (±%.2f) %7.2f (±%.2f) %11.2fx %10.1f%%\n" \
            "$model_name" "$b_sim" "$b_sim_std" "$c_sim" "$c_sim_std" "$sim_speedup" "$sim_improvement"
    else
        printf "%-20s %15s %15s %12s %12s\n" "$model_name" "N/A" "N/A" "-" "-"
    fi
done

echo ""
echo -e "${GREEN}Benchmark complete!${NC}"
echo ""

# Save results to text file
{
    echo "BioNetGen Performance Benchmark"
    echo "Date: $(date)"
    echo "Baseline:    $BASELINE_REF ($(git rev-parse --short $BASELINE_REF))"
    echo "Comparison:  $COMPARISON_REF ($(git rev-parse --short $COMPARISON_REF))"
    echo "Runs per model: $RUNS"
    echo ""
    echo "Overall Performance:"
    printf "%-20s %15s %15s %12s %12s\n" "Model" "Baseline (s)" "Comparison (s)" "Speedup" "Improvement"
    echo "------------------------------------------------------------------------------------"

    for i in "${!results_names[@]}"; do
        model_name="${results_names[$i]}"
        IFS=',' read b_wall b_wall_std b_gen b_gen_std b_sim b_sim_std b_cpu b_cpu_std <<< "${results_baseline_all[$i]}"
        IFS=',' read c_wall c_wall_std c_gen c_gen_std c_sim c_sim_std c_cpu c_cpu_std <<< "${results_comparison_all[$i]}"

        speedup=$(perl -e "print $b_wall / $c_wall")
        improvement=$(perl -e "print (($b_wall - $c_wall) / $b_wall) * 100")

        printf "%-20s %7.2f (±%.2f) %7.2f (±%.2f) %11.2fx %10.1f%%\n" \
            "$model_name" "$b_wall" "$b_wall_std" "$c_wall" "$c_wall_std" "$speedup" "$improvement"
    done

    echo ""
    echo "Phase Breakdown - generate_network():"
    printf "%-20s %15s %15s %12s %12s\n" "Model" "Baseline (s)" "Comparison (s)" "Speedup" "Improvement"
    echo "------------------------------------------------------------------------------------"

    for i in "${!results_names[@]}"; do
        model_name="${results_names[$i]}"
        IFS=',' read b_wall b_wall_std b_gen b_gen_std b_sim b_sim_std b_cpu b_cpu_std <<< "${results_baseline_all[$i]}"
        IFS=',' read c_wall c_wall_std c_gen c_gen_std c_sim c_sim_std c_cpu c_cpu_std <<< "${results_comparison_all[$i]}"

        if [ "$(perl -e "print $b_gen > 0 ? 1 : 0")" = "1" ]; then
            gen_speedup=$(perl -e "print $b_gen / $c_gen")
            gen_improvement=$(perl -e "print (($b_gen - $c_gen) / $b_gen) * 100")
            printf "%-20s %7.2f (±%.2f) %7.2f (±%.2f) %11.2fx %10.1f%%\n" \
                "$model_name" "$b_gen" "$b_gen_std" "$c_gen" "$c_gen_std" "$gen_speedup" "$gen_improvement"
        else
            printf "%-20s %15s %15s %12s %12s\n" "$model_name" "N/A" "N/A" "-" "-"
        fi
    done

    echo ""
    echo "Phase Breakdown - simulate():"
    printf "%-20s %15s %15s %12s %12s\n" "Model" "Baseline (s)" "Comparison (s)" "Speedup" "Improvement"
    echo "------------------------------------------------------------------------------------"

    for i in "${!results_names[@]}"; do
        model_name="${results_names[$i]}"
        IFS=',' read b_wall b_wall_std b_gen b_gen_std b_sim b_sim_std b_cpu b_cpu_std <<< "${results_baseline_all[$i]}"
        IFS=',' read c_wall c_wall_std c_gen c_gen_std c_sim c_sim_std c_cpu c_cpu_std <<< "${results_comparison_all[$i]}"

        if [ "$(perl -e "print $b_sim > 0 ? 1 : 0")" = "1" ]; then
            sim_speedup=$(perl -e "print $b_sim / $c_sim")
            sim_improvement=$(perl -e "print (($b_sim - $c_sim) / $b_sim) * 100")
            printf "%-20s %7.2f (±%.2f) %7.2f (±%.2f) %11.2fx %10.1f%%\n" \
                "$model_name" "$b_sim" "$b_sim_std" "$c_sim" "$c_sim_std" "$sim_speedup" "$sim_improvement"
        else
            printf "%-20s %15s %15s %12s %12s\n" "$model_name" "N/A" "N/A" "-" "-"
        fi
    done
} > "$RESULTS_TXT"

# Save results to CSV
{
    echo "Model,Phase,Baseline_Mean,Baseline_StdDev,Comparison_Mean,Comparison_StdDev,Speedup,Improvement_Percent"

    for i in "${!results_names[@]}"; do
        model_name="${results_names[$i]}"

        # Parse results
        IFS=',' read b_wall b_wall_std b_gen b_gen_std b_sim b_sim_std b_cpu b_cpu_std <<< "${results_baseline_all[$i]}"
        IFS=',' read c_wall c_wall_std c_gen c_gen_std c_sim c_sim_std c_cpu c_cpu_std <<< "${results_comparison_all[$i]}"

        # Overall (wall clock)
        speedup=$(perl -e "print $b_wall / $c_wall")
        improvement=$(perl -e "print (($b_wall - $c_wall) / $b_wall) * 100")
        echo "$model_name,overall,$b_wall,$b_wall_std,$c_wall,$c_wall_std,$speedup,$improvement"

        # Generate network phase
        if [ "$(perl -e "print $b_gen > 0 ? 1 : 0")" = "1" ]; then
            gen_speedup=$(perl -e "print $b_gen / $c_gen")
            gen_improvement=$(perl -e "print (($b_gen - $c_gen) / $b_gen) * 100")
            echo "$model_name,generate_network,$b_gen,$b_gen_std,$c_gen,$c_gen_std,$gen_speedup,$gen_improvement"
        fi

        # Simulate phase
        if [ "$(perl -e "print $b_sim > 0 ? 1 : 0")" = "1" ]; then
            sim_speedup=$(perl -e "print $b_sim / $c_sim")
            sim_improvement=$(perl -e "print (($b_sim - $c_sim) / $b_sim) * 100")
            echo "$model_name,simulate,$b_sim,$b_sim_std,$c_sim,$c_sim_std,$sim_speedup,$sim_improvement"
        fi
    done
} > "$RESULTS_CSV"

echo "Results saved to:"
echo "  $RESULTS_TXT"
echo "  $RESULTS_CSV"
