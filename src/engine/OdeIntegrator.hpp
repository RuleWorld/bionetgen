#pragma once

#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "ast/Model.hpp"
#include "NetworkGenerator.hpp"

namespace bng::engine {

struct OdeOptions {
    double tStart = 0.0;
    double tEnd = 1.0;
    std::size_t nSteps = 100;
    double rtol = 1e-8;
    double atol = 1e-8;
    std::string method = "euler";  // "euler", "rk4", "cvode" (cvode not yet implemented)
    double maxStep = 0.0;          // 0 = no limit
    std::string stopCondition;     // muParser expression (empty = none)
    unsigned int seed = 0;         // for SSA (0 = random)
    bool steadyState = false;      // Enable steady-state detection
    double steadyStateTol = 1e-8;  // Tolerance for steady-state (|dydt| < tol)
    std::string stopIf;            // Boolean expression to evaluate at each step
    bool printCDAT = true;         // Whether to write .cdat output file
    bool printFunctions = false;   // Whether to write .fdat output file
    std::vector<double> sampleTimes; // Non-uniform output time points (overrides nSteps)
    std::size_t maxSimSteps = 0;   // Max internal simulation steps (0 = unlimited)
    bool saveProgress = false;     // Write .net checkpoint at each output step
    bool printNet = false;         // Write .net file after simulation with final concentrations
    bool printEnd = false;         // Output final state when simulation stops (stop_if or steady_state)
    std::size_t outputStepInterval = 0; // Output every N internal steps (0 = disabled, use n_steps timing)
    std::string netfile;           // Custom .net file to read instead of auto-generating

    // Named kinetic parameters to compute forward sensitivities for (method="cvode" only).
    // Empty = ordinary simulation, no sensitivity system integrated.
    // Each name must appear in some reaction's rate law as a runtime-evaluated
    // parameter reference (plain mass-action k, or Sat/MM/Hill/Arrhenius args).
    // Parameters that only ever reach the solver as an already-baked, per-reaction
    // derived numeric rate (energy patterns, %x:: local functions) are not supported
    // yet and will throw at integrate() time.
    std::vector<std::string> sensParams;
    // Fitting routines almost always only need d(observable)/d(param), not
    // d(species)/d(param) for every hidden species. Set to false to skip
    // storing/copying speciesSensitivities each output step -- for a network
    // with many species and several fit parameters over many output steps,
    // this avoids the dominant memory/copy cost of a sensitivity run.
    bool sensSpeciesOutput = true;
};

struct OdeResult {
    std::vector<double> timePoints;                    // length = nSteps + 1
    std::vector<std::vector<double>> concentrations;   // [timeIndex][speciesIndex]
    std::vector<std::vector<double>> observables;      // [timeIndex][groupIndex]

    // Forward sensitivities, present only if OdeOptions::sensParams was non-empty.
    // Outer index matches sensParamNames (same order as requested).
    std::vector<std::string> sensParamNames;
    std::vector<std::vector<std::vector<double>>> speciesSensitivities;    // [paramIdx][timeIndex][speciesIndex] = d(conc)/d(param)
    std::vector<std::vector<std::vector<double>>> observableSensitivities; // [paramIdx][timeIndex][groupIndex]   = d(obs)/d(param)
};

// One observed data point for adjoint gradient fitting: contributes
// weight*(model(time,observable) - value)^2 to the loss.
struct AdjointDataPoint {
    double time;
    std::string observable;  // must match a name in the model's observables
    double value;
    double weight = 1.0;
};

struct AdjointGradientResult {
    double loss = 0.0;
    std::vector<std::string> paramNames;
    std::vector<double> gradient;  // dLoss/dparam, same order as paramNames
};

class OdeIntegrator {
public:
    OdeIntegrator(const ast::Model& model, const GeneratedNetwork& network);
    ~OdeIntegrator();

    OdeResult integrate(const OdeOptions& options);
    void writeOutputFiles(const std::string& prefix, const OdeResult& result, bool printCDAT = true, bool printFunctions = false, bool append = false) const;
    // Writes one .csc (species sensitivities) and one .gsc (observable
    // sensitivities) file per entry in result.sensParamNames, in the same
    // row/column layout as the existing LinearParameterSensitivity action's
    // finite-difference output ("# time" header row, one row per species/
    // observable). Species rows are labeled "1".."N" to match .cdat's
    // headerless convention; observable rows use compiledGroups_ names to
    // match .gdat's header. No-op if result.sensParamNames is empty.
    void writeSensitivityFiles(const std::string& prefix, const OdeResult& result, const std::string& suffix = "") const;
    void derivs(double t, const double* y, double* dydt) const;

    // Adjoint sensitivity via CVODES (Froehlich et al. 2017-style): computes the
    // gradient of a weighted-least-squares objective against externally supplied
    // data in cost that does NOT scale with the number of fit parameters, unlike
    // integrateCvodesForwardSens (forward sensitivity), whose cost scales
    // ~linearly with parameter count. Trade-off: this only returns the scalar
    // loss and its gradient, not full d(species)/d(param) trajectories, and it
    // needs an explicit dataset rather than working for an arbitrary simulation.
    //
    // paramValues (same length/order as opts.sensParams) lets the caller
    // evaluate at parameter values other than whatever is currently in the
    // model -- the natural shape for an optimizer's objective function, which
    // needs to try many candidate vectors without mutating shared model state.
    //
    // IMPORTANT COST CAVEAT (this implementation, not the general method):
    // AMICI's adjoint mode is cheap for large parameter counts because it
    // supplies an analytic/sparse model Jacobian via symbolic code generation.
    // Nothing in this codebase computes an analytic Jacobian anywhere, so the
    // backward (adjoint) ODE's Jacobian here is built via finite differences on
    // derivs() (cached per distinct integration time, and handed to CVODES as
    // an explicit dense Jacobian to avoid it re-deriving the same thing via its
    // own nested internal differencing). That keeps cost the same *order* as
    // the forward dense solver (~nSpecies extra derivs() calls per Jacobian
    // rebuild), but it means this still scales with the number of SPECIES --
    // just not with the number of PARAMETERS. In other words: this only pays
    // off once the fit-parameter count is large *relative to the species
    // count*, a materially higher bar than the textbook "adjoint wins for many
    // parameters" story. For typical BNGL parameter counts (dozens),
    // integrateCvodesForwardSens is almost certainly faster and simpler.
    AdjointGradientResult computeAdjointGradient(const OdeOptions& opts,
                                                  const std::vector<double>& paramValues,
                                                  const std::vector<AdjointDataPoint>& data);

private:
    const ast::Model& model_;
    const GeneratedNetwork& network_;

    // Compiled reaction network for fast derivative evaluation
    struct CompiledReaction {
        std::vector<std::size_t> reactantIndices;  // 0-based species indices
        std::vector<std::size_t> productIndices;
        double rateConstant;          // evaluated rate (for elementary)
        double statFactor;            // statistical factor
        bool isFunctional = false;    // true if rate depends on time/observables
        std::optional<ast::Expression> functionalRateExpr;  // for runtime evaluation
        bool isTotalRate = false;     // true if rate is total (not multiplied by reactant conc)

        // Fast path for the common case: rate law is exactly a bare sensitivity
        // parameter name (e.g. "A+B->C k1" with k1 selected for sensitivity).
        // Set instead of isFunctional/functionalRateExpr -- avoids Expression
        // tree evaluation (resolver lambda, hash lookups, std::function calls)
        // on every derivs() call for what is otherwise just rate = scale * k.
        std::optional<std::size_t> sensParamDirectIdx;  // index into activeSensP_
        double sensParamDirectScale = 1.0;              // unitFactor * statFactor
    };

    struct CompiledGroup {
        std::string name;
        std::vector<std::pair<std::size_t, double>> entries;  // (speciesIndex, weight)
    };

    std::vector<CompiledReaction> compiledRxns_;
    std::vector<CompiledGroup> compiledGroups_;
    std::vector<bool> fixedSpecies_;    // true for $ (constant) species
    std::size_t nSpecies_ = 0;
    bool hasFunctionalRates_ = false;

    // Performance optimizations
    mutable std::vector<double> groupValues_;                  // Pre-allocated for derivs()
    std::unordered_map<std::string, std::size_t> observableIndex_; // O(1) observable lookup
    std::vector<std::size_t> constantRxnIndices_;              // Indices of constant-rate reactions
    std::vector<std::size_t> functionalRxnIndices_;            // Indices of functional-rate reactions
    std::vector<std::size_t> directSensRxnIndices_;            // Indices of bare-sensitivity-parameter reactions (fast path)
    std::unordered_map<std::string, const ast::Expression*> functionMap_; // O(1) function lookup
    std::vector<std::vector<std::size_t>> ssaRxnUpdateRxn_;    // Reaction dependency graph for SSA

    // Forward-sensitivity support. When non-empty, compile() forces every reaction
    // whose rate law mentions one of these names to go through the runtime
    // (functional) evaluation path instead of being pre-baked to a double, so that
    // derivs()'s resolver can substitute a live, CVODES-owned parameter value.
    std::vector<std::string> sensParamNames_;
    std::unordered_map<std::string, std::size_t> sensParamIndex_;
    // Non-null only while integrateCvodesForwardSens() is running; points at the
    // pVec buffer CVODES perturbs in place for its internal difference-quotient
    // sensitivity RHS. derivs()'s resolver checks this before falling back to
    // model_.getParameters().evaluate(name).
    const double* activeSensP_ = nullptr;
    // Tracks which sensParams compile() was last built for, so integrate() only
    // pays for a recompile when the requested parameter set actually changes.
    std::vector<std::string> lastCompiledSensParams_;

    void ensureCompiledFor(const std::vector<std::string>& sensParams);

    void compile();
    void compileGroups();
    void updateGroups(const double* y, std::vector<double>& groupValues) const;

    OdeResult integrateEuler(const OdeOptions& opts);
    OdeResult integrateRK4(const OdeOptions& opts);
    OdeResult integrateCvode(const OdeOptions& opts);
    OdeResult integrateCvodesForwardSens(const OdeOptions& opts);
    OdeResult integrateSSA(const OdeOptions& opts);

    double computePropensity(const CompiledReaction& rxn, const std::vector<double>& y) const;
};

} // namespace bng::engine
