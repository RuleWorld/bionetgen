#!/usr/bin/env bash
# BioNetGen Performance Benchmark Suite v1.3
# Compares performance and memory between two git references
# Compatible with bash 3.2+

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

# Default configuration
DEFAULT_MODELS=(
    "bng2/Models2/egfr_net.bngl"
    "bng2/Models2/fceri_ji.bngl"
    "bng2/Models2/FceriModels/fceri_fyn.bngl"
    "bng2/Validate/gene_expr.bngl"
    "bng2/Models2/simple_nfsim.bngl"
)

# Ensure models needed for --validate are in bng2/Validate
for m in "${DEFAULT_MODELS[@]}"; do
    dest="bng2/Validate/$(basename "$m")"
    if [ ! -f "$dest" ]; then
        cp "$m" "$dest"
    fi
done
RUNS=3
REBUILD_NETWORK3=auto
VALIDATE=false

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
  --models M1,M2    Comma-separated model names or paths
  --rebuild         Force rebuild of binaries
  --no-rebuild      Skip rebuilding binaries
  --validate        Run validation suite for benchmarked models
  -h, --help        Show this help message

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
            IFS=',' read -ra MODELS_INPUT <<< "$2"
            MODELS=()
            for m in "${MODELS_INPUT[@]}"; do
                FOUND=false
                for try in "$m" "${m}.bngl"; do
                    if [ -f "$try" ]; then MODELS+=("$try"); FOUND=true; break; fi
                done
                [ "$FOUND" = true ] && continue
                for dir in "bng2/Models2" "bng2/Models2/FceriModels" "bng2/Validate"; do
                    for try in "$dir/$m" "$dir/${m}.bngl"; do
                        if [ -f "$try" ]; then MODELS+=("$try"); FOUND=true; break 2; fi
                    done
                done
                if [ "$FOUND" = false ]; then MODELS+=("$m"); fi
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
        --validate)
            VALIDATE=true
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

RESULTS_DIR="$SCRIPT_DIR/results"
mkdir -p "$RESULTS_DIR"
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

# Verify git refs
git rev-parse --verify "$BASELINE_REF" >/dev/null 2>&1
git rev-parse --verify "$COMPARISON_REF" >/dev/null 2>&1

# Verify models
for model in "${MODELS[@]}"; do
    if [ ! -f "$model" ]; then
        echo -e "${RED}ERROR: Model not found: $model${NC}"
        exit 1
    fi
done

ORIGINAL_REF=$(git symbolic-ref -q HEAD 2>/dev/null || git rev-parse HEAD)

cleanup() {
    echo ""
    echo "Cleaning up..."
    git checkout "$ORIGINAL_REF" --quiet 2>/dev/null || true
    git clean -fd bng2/Models2 > /dev/null 2>&1 || true
}
trap cleanup EXIT

needs_rebuild() {
    if [ "$REBUILD_NETWORK3" = "always" ]; then return 0; fi
    if [ "$REBUILD_NETWORK3" = "never" ]; then return 1; fi
    if git diff --name-only "$1" "$2" | grep -q "bng2/Network3/.*\.\(cpp\|h\|hpp\|c\)$\|bng-graph/.*\.\(cpp\|h\|hpp\)$\|bng2/nfsim_src/.*"; then
        return 0
    else
        return 1
    fi
}

rebuild_binaries() {
    echo -n "    Rebuilding BioNetGen binaries... "
    cd bng2
    make clean > /dev/null 2>&1 || true
    if make > /dev/null 2>&1; then
        if [ -d "nfsim_src" ]; then
            cd nfsim_src
            make clean > /dev/null 2>&1 || true
            if make > /dev/null 2>&1; then
                cp bin/NFsim ../bin/NFsim
            fi
        fi
        cd "$REPO_ROOT"
        echo "done"
        return 0
    else
        cd "$REPO_ROOT"
        echo -e "${RED}FAILED${NC}"
        return 1
    fi
}

run_benchmark() {
    local model="$1"
    local log_file="$2"
    local rss_file="${log_file}.rss"

    local start=$(perl -MTime::HiRes -e 'print Time::HiRes::time()')
    python3 "$SCRIPT_DIR/peak_rss.py" perl bng2/BNG2.pl "$model" > "$log_file" 2> "$rss_file"
    local end=$(perl -MTime::HiRes -e 'print Time::HiRes::time()')

    local wallclock=$(perl -e "print $end - $start")
    local rss=$(grep "PEAK_RSS_KB" "$rss_file" | awk '{print $2}' || echo "0")
    local generate_time=$(grep "CPU TIME: generate_network" "$log_file" | tail -n 1 | awk '{print $4}' || echo "0")
    local simulate_time=$(grep "CPU TIME: simulate" "$log_file" | tail -n 1 | awk '{print $4}' || echo "0")
    local total_cpu_time=$(grep "CPU TIME: total" "$log_file" | tail -n 1 | awk '{print $4}' || echo "0")

    local nf_events=$(grep "You just simulated" "$log_file" | awk '{print $4}' | head -n 1 || echo "0")
    local nf_sim_time=$(grep "You just simulated" "$log_file" | awk '{print $7}' | head -n 1 || echo "0")
    local events_per_sec=0
    if [ "$nf_sim_time" != "0" ] && [ "$nf_sim_time" != "" ] && [ "$nf_events" != "0" ] && [ "$nf_events" != "" ]; then
        events_per_sec=$(perl -e "if (\$ARGV[0] > 0) { print \$ARGV[1] / \$ARGV[0] } else { print 0 }" "$nf_sim_time" "$nf_events")
    fi

    echo "$wallclock,$generate_time,$simulate_time,$total_cpu_time,$rss,$events_per_sec"
}

calc_average() {
    local sum=0; local count=0
    for val in "$@"; do sum=$(perl -e "print $sum + $val"); count=$((count + 1)); done
    if [ "$count" -eq 0 ]; then echo "0"; else perl -e "print $sum / $count"; fi
}

calc_stddev() {
    local mean="$1"; shift; local sum_sq=0; local count=0
    for val in "$@"; do sum_sq=$(perl -e "print $sum_sq + ($val - $mean) ** 2"); count=$((count + 1)); done
    if [ "$count" -eq 0 ]; then echo "0"; else perl -e "print sqrt($sum_sq / $count)"; fi
}

NEEDS_REBUILD=false
if needs_rebuild "$BASELINE_REF" "$COMPARISON_REF"; then NEEDS_REBUILD=true; fi

results_names=(); results_baseline_all=(); results_comparison_all=(); results_validation=()
TEMP_LOG_DIR=$(mktemp -d "${TMPDIR:-/tmp}/bng_benchmark.XXXXXX")
trap 'rm -rf "$TEMP_LOG_DIR"; cleanup' EXIT

for model in "${MODELS[@]}"; do results_names+=("$(basename "$model" .bngl)"); done

# --- Baseline ---
echo -e "${YELLOW}=== Benchmarking Baseline: $BASELINE_REF ===${NC}"
git checkout "$BASELINE_REF" --quiet 2>/dev/null
if [ "$NEEDS_REBUILD" = true ]; then rebuild_binaries; fi

for model in "${MODELS[@]}"; do
    model_name=$(basename "$model" .bngl)
    echo -n "  $model_name: "
    wc_t=(); gen_t=(); sim_t=(); cpu_t=(); rss_v=(); eps_v=()
    for run in $(seq 1 $RUNS); do
        res=$(run_benchmark "$model" "$TEMP_LOG_DIR/b_${model_name}_${run}.log")
        IFS=',' read wc gen sim cpu rss eps <<< "$res"
        wc_t+=($wc); gen_t+=($gen); sim_t+=($sim); cpu_t+=($cpu); rss_v+=($rss); eps_v+=($eps)
        echo -n "."
    done
    avg_wc=$(calc_average "${wc_t[@]}"); std_wc=$(calc_stddev "$avg_wc" "${wc_t[@]}")
    avg_gen=$(calc_average "${gen_t[@]}"); std_gen=$(calc_stddev "$avg_gen" "${gen_t[@]}")
    avg_sim=$(calc_average "${sim_t[@]}"); std_sim=$(calc_stddev "$avg_sim" "${sim_t[@]}")
    avg_cpu=$(calc_average "${cpu_t[@]}"); std_cpu=$(calc_stddev "$avg_cpu" "${cpu_t[@]}")
    avg_rss=$(calc_average "${rss_v[@]}"); std_rss=$(calc_stddev "$avg_rss" "${rss_v[@]}")
    avg_eps=$(calc_average "${eps_v[@]}"); std_eps=$(calc_stddev "$avg_eps" "${eps_v[@]}")
    echo " wall=${avg_wc}s gen=${avg_gen}s sim=${avg_sim}s rss=${avg_rss}KB"
    results_baseline_all+=("$avg_wc,$std_wc,$avg_gen,$std_gen,$avg_sim,$std_sim,$avg_cpu,$std_cpu,$avg_rss,$std_rss,$avg_eps,$std_eps")
done

# --- Comparison ---
echo -e "${YELLOW}=== Benchmarking Comparison: $COMPARISON_REF ===${NC}"
git checkout "$COMPARISON_REF" --quiet 2>/dev/null
if [ "$NEEDS_REBUILD" = true ]; then rebuild_binaries; fi

for i in "${!MODELS[@]}"; do
    model="${MODELS[$i]}"
    model_name=$(basename "$model" .bngl)
    echo -n "  $model_name: "
    wc_t=(); gen_t=(); sim_t=(); cpu_t=(); rss_v=(); eps_v=()
    for run in $(seq 1 $RUNS); do
        res=$(run_benchmark "$model" "$TEMP_LOG_DIR/c_${model_name}_${run}.log")
        IFS=',' read wc gen sim cpu rss eps <<< "$res"
        wc_t+=($wc); gen_t+=($gen); sim_t+=($sim); cpu_t+=($cpu); rss_v+=($rss); eps_v+=($eps)
        echo -n "."
    done
    avg_wc=$(calc_average "${wc_t[@]}"); std_wc=$(calc_stddev "$avg_wc" "${wc_t[@]}")
    avg_gen=$(calc_average "${gen_t[@]}"); std_gen=$(calc_stddev "$avg_gen" "${gen_t[@]}")
    avg_sim=$(calc_average "${sim_t[@]}"); std_sim=$(calc_stddev "$avg_sim" "${sim_t[@]}")
    avg_cpu=$(calc_average "${cpu_t[@]}"); std_cpu=$(calc_stddev "$avg_cpu" "${cpu_t[@]}")
    avg_rss=$(calc_average "${rss_v[@]}"); std_rss=$(calc_stddev "$avg_rss" "${rss_v[@]}")
    avg_eps=$(calc_average "${eps_v[@]}"); std_eps=$(calc_stddev "$avg_eps" "${eps_v[@]}")
    echo " wall=${avg_wc}s gen=${avg_gen}s sim=${avg_sim}s rss=${avg_rss}KB"
    results_comparison_all+=("$avg_wc,$std_wc,$avg_gen,$std_gen,$avg_sim,$std_sim,$avg_cpu,$std_cpu,$avg_rss,$std_rss,$avg_eps,$std_eps")

    if [ "$VALIDATE" = true ]; then
        echo -n "    Validating... "
        if perl bng2/Validate/validate_examples.pl "$model_name" > /dev/null 2>&1; then echo -e "${GREEN}PASSED${NC}"; results_validation+=("PASSED"); else echo -e "${RED}FAILED${NC}"; results_validation+=("FAILED"); fi
    else results_validation+=("N/A"); fi
done

# --- Summary ---
{
    echo "Overall Performance:"
    printf "%-20s %15s %15s %12s %12s %10s\n" "Model" "Baseline (s)" "Comparison (s)" "Speedup" "Improvement" "Validation"
    echo "------------------------------------------------------------------------------------------------"
    for i in "${!results_names[@]}"; do
        name="${results_names[$i]}"
        IFS=',' read b_wc b_wc_s b_gen b_gen_s b_sim b_sim_s b_cpu b_cpu_s b_rss b_rss_s b_eps b_eps_s <<< "${results_baseline_all[$i]}"
        IFS=',' read c_wc c_wc_s c_gen c_gen_s c_sim c_sim_s c_cpu c_cpu_s c_rss c_rss_s c_eps c_eps_s <<< "${results_comparison_all[$i]}"
        speedup=$(perl -e "print $b_wc / $c_wc")
        improvement=$(perl -e "printf '%.1f', (($b_wc - $c_wc) / $b_wc) * 100")
        printf "%-20s %7.2f (±%.2f) %7.2f (±%.2f) %11.2fx %11s%% %10s\n" "$name" "$b_wc" "$b_wc_s" "$c_wc" "$c_wc_s" "$speedup" "$improvement" "${results_validation[$i]}"
    done

    echo -e "\nMemory Usage (Peak RSS):"
    printf "%-20s %15s %15s %12s %12s\n" "Model" "Baseline (KB)" "Comparison (KB)" "Ratio" "Change"
    echo "------------------------------------------------------------------------------------"
    for i in "${!results_names[@]}"; do
        name="${results_names[$i]}"
        IFS=',' read b_wc b_wc_s b_gen b_gen_s b_sim b_sim_s b_cpu b_cpu_s b_rss b_rss_s b_eps b_eps_s <<< "${results_baseline_all[$i]}"
        IFS=',' read c_wc c_wc_s c_gen c_gen_s c_sim c_sim_s c_cpu c_cpu_s c_rss c_rss_s c_eps c_eps_s <<< "${results_comparison_all[$i]}"
        if [ "$(perl -e "print $b_rss > 0 ? 1 : 0")" = "1" ]; then ratio=$(perl -e "print $c_rss / $b_rss"); change=$(perl -e "printf '%.1f', (($c_rss - $b_rss) / $b_rss) * 100"); printf "%-20s %7.0f (±%.0f) %7.0f (±%.0f) %11.2fx %11s%%\n" "$name" "$b_rss" "$b_rss_s" "$c_rss" "$c_rss_s" "$ratio" "$change"; fi
    done

    echo -e "\nSimulation Throughput (NFsim events/sec):"
    printf "%-20s %15s %15s %12s %12s\n" "Model" "Baseline" "Comparison" "Speedup" "Improvement"
    echo "------------------------------------------------------------------------------------"
    for i in "${!results_names[@]}"; do
        name="${results_names[$i]}"
        IFS=',' read b_wc b_wc_s b_gen b_gen_s b_sim b_sim_s b_cpu b_cpu_s b_rss b_rss_s b_eps b_eps_s <<< "${results_baseline_all[$i]}"
        IFS=',' read c_wc c_wc_s c_gen c_gen_s c_sim c_sim_s c_cpu c_cpu_s c_rss c_rss_s c_eps c_eps_s <<< "${results_comparison_all[$i]}"
        if [ "$(perl -e "print $b_eps > 0 ? 1 : 0")" = "1" ]; then speedup=$(perl -e "print $c_eps / $b_eps"); improvement=$(perl -e "printf '%.1f', (($c_eps - $b_eps) / $b_eps) * 100"); printf "%-20s %7.1f (±%.1f) %7.1f (±%.1f) %11.2fx %11s%%\n" "$name" "$b_eps" "$b_eps_s" "$c_eps" "$c_eps_s" "$speedup" "$improvement"; fi
    done
} | tee "$RESULTS_TXT"

# Save CSV
{
    echo "Model,Phase,Baseline_Mean,Baseline_StdDev,Comparison_Mean,Comparison_StdDev,Speedup,Improvement_Percent"
    for i in "${!results_names[@]}"; do
        name="${results_names[$i]}"
        IFS=',' read b_wc b_wc_s b_gen b_gen_s b_sim b_sim_s b_cpu b_cpu_s b_rss b_rss_s b_eps b_eps_s <<< "${results_baseline_all[$i]}"
        IFS=',' read c_wc c_wc_s c_gen c_gen_s c_sim c_sim_s c_cpu c_cpu_s c_rss c_rss_s c_eps c_eps_s <<< "${results_comparison_all[$i]}"
        echo "$name,overall,$b_wc,$b_wc_s,$c_wc,$c_wc_s,$(perl -e "print $b_wc / $c_wc"),$(perl -e "printf '%.1f', (($b_wc - $c_wc) / $b_wc) * 100")"
        if [ "$(perl -e "print $b_gen > 0 ? 1 : 0")" = "1" ] && [ "$(perl -e "print $c_gen > 0 ? 1 : 0")" = "1" ]; then echo "$name,generate_network,$b_gen,$b_gen_s,$c_gen,$c_gen_s,$(perl -e "print $b_gen / $c_gen"),$(perl -e "printf '%.1f', (($b_gen - $c_gen) / $b_gen) * 100")"; fi
        if [ "$(perl -e "print $b_sim > 0 ? 1 : 0")" = "1" ] && [ "$(perl -e "print $c_sim > 0 ? 1 : 0")" = "1" ]; then echo "$name,simulate,$b_sim,$b_sim_s,$c_sim,$c_sim_s,$(perl -e "print $b_sim / $c_sim"),$(perl -e "printf '%.1f', (($b_sim - $c_sim) / $b_sim) * 100")"; fi
        if [ "$(perl -e "print $b_rss > 0 ? 1 : 0")" = "1" ]; then echo "$name,peak_rss,$b_rss,$b_rss_s,$c_rss,$c_rss_s,$(perl -e "print $c_rss / $b_rss"),$(perl -e "printf '%.1f', (($c_rss - $b_rss) / $b_rss) * 100")"; fi
        if [ "$(perl -e "print $b_eps > 0 ? 1 : 0")" = "1" ]; then echo "$name,nfsim_eps,$b_eps,$b_eps_s,$c_eps,$c_eps_s,$(perl -e "print $c_eps / $b_eps"),$(perl -e "printf '%.1f', (($c_eps - $b_eps) / $b_eps) * 100")"; fi
    done
} > "$RESULTS_CSV"

echo -e "\n${GREEN}Benchmark complete!${NC}"
echo "Results saved to: $RESULTS_TXT and $RESULTS_CSV"
