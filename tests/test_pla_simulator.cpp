#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "../src/engine/PlaSimulator.hpp"
#include "../src/ast/Model.hpp"
#include "../src/engine/NetworkGenerator.hpp"
// Include implementation to exercise evaluateRateString in anonymous namespace.
#include "../src/engine/PlaSimulator.cpp"

using namespace bng::engine;
using namespace bng::ast;

TEST_CASE("PlaConfig parsing", "[PlaSimulator]") {
    SECTION("Default configuration") {
        PlaConfig config;
        REQUIRE(config.method == PlaConfig::Method::fEuler);
        REQUIRE(config.checkType == PlaConfig::CheckType::PreNeg);
        REQUIRE(config.basis == PlaConfig::Basis::SpeciesBased);
        REQUIRE_THAT(config.eps, Catch::Matchers::WithinAbs(0.03, 1e-6));
        REQUIRE_THAT(config.p, Catch::Matchers::WithinAbs(0.5, 1e-6));
        REQUIRE_THAT(config.q, Catch::Matchers::WithinAbs(1.5, 1e-6));
        REQUIRE_THAT(config.w, Catch::Matchers::WithinAbs(0.75, 1e-6));
        REQUIRE_THAT(config.fixedTau, Catch::Matchers::WithinAbs(0.0, 1e-6));
    }

    SECTION("Parsing simple format") {
        PlaConfig config = PlaConfig::parse("fEuler|pre-neg:sb|eps=0.05");
        REQUIRE(config.method == PlaConfig::Method::fEuler);
        REQUIRE(config.checkType == PlaConfig::CheckType::PreNeg);
        REQUIRE(config.basis == PlaConfig::Basis::SpeciesBased);
        REQUIRE_THAT(config.eps, Catch::Matchers::WithinAbs(0.05, 1e-6));
    }

    SECTION("Parsing all methods") {
        REQUIRE(PlaConfig::parse("fEuler||").method == PlaConfig::Method::fEuler);
        REQUIRE(PlaConfig::parse("midpt||").method == PlaConfig::Method::Midpoint);
        REQUIRE(PlaConfig::parse("rk4||").method == PlaConfig::Method::RK4);
    }

    SECTION("Parsing check types and basis") {
        auto c1 = PlaConfig::parse("|pre-neg:sb|");
        REQUIRE(c1.checkType == PlaConfig::CheckType::PreNeg);
        REQUIRE(c1.basis == PlaConfig::Basis::SpeciesBased);

        auto c2 = PlaConfig::parse("|post-eps:rb|");
        REQUIRE(c2.checkType == PlaConfig::CheckType::PostEps);
        REQUIRE(c2.basis == PlaConfig::Basis::ReactionBased);
    }

    SECTION("Parsing multiple parameters") {
        PlaConfig config = PlaConfig::parse("||eps=0.1,tau=1.5,p=0.8,q=2.0,w=0.5");
        REQUIRE_THAT(config.eps, Catch::Matchers::WithinAbs(0.1, 1e-6));
        REQUIRE_THAT(config.fixedTau, Catch::Matchers::WithinAbs(1.5, 1e-6));
        REQUIRE_THAT(config.p, Catch::Matchers::WithinAbs(0.8, 1e-6));
        REQUIRE_THAT(config.q, Catch::Matchers::WithinAbs(2.0, 1e-6));
        REQUIRE_THAT(config.w, Catch::Matchers::WithinAbs(0.5, 1e-6));
    }

    SECTION("Handling malformed inputs gracefully") {
        PlaConfig c1 = PlaConfig::parse("invalidMethod|pre-neg:sb|eps=0.05");
        REQUIRE(c1.method == PlaConfig::Method::fEuler);

        PlaConfig c2 = PlaConfig::parse("||eps=invalid,tau=malformed,p=2");
        REQUIRE_THAT(c2.eps, Catch::Matchers::WithinAbs(0.03, 1e-6));
        REQUIRE_THAT(c2.fixedTau, Catch::Matchers::WithinAbs(0.0, 1e-6));
        REQUIRE_THAT(c2.p, Catch::Matchers::WithinAbs(2.0, 1e-6));
    }
}

TEST_CASE("PlaSimulator initialization and simulation", "[PlaSimulator]") {
    Model model;
    GeneratedNetwork network;

    // Test that empty simulation completes successfully
    PlaSimulator simulator(model, network);

    OdeOptions opts;
    opts.tEnd = 1.0;
    opts.nSteps = 10;

    PlaConfig config;

    OdeResult result = simulator.simulate(opts, config);

    REQUIRE(result.timePoints.size() > 0);
    REQUIRE_THAT(result.timePoints.back(), Catch::Matchers::WithinAbs(1.0, 1e-6));
    REQUIRE(result.concentrations.size() == result.timePoints.size());
    REQUIRE(result.observables.size() == result.timePoints.size());
}

TEST_CASE("evaluateRateString error path", "[PlaSimulator]") {
    auto resolve = [](const std::string& s) {
        if (s == "valid_var") return 42.0;
        return 0.0;
    };

    REQUIRE_THAT(evaluateRateString("1.23", resolve), Catch::Matchers::WithinRel(1.23));
    REQUIRE_THAT(evaluateRateString("invalid_float", resolve), Catch::Matchers::WithinRel(0.0));
    REQUIRE_THAT(evaluateRateString("valid_var", resolve), Catch::Matchers::WithinRel(42.0));
    REQUIRE_THAT(evaluateRateString("1.23abc", resolve), Catch::Matchers::WithinRel(0.0));
}

namespace bng::engine {
class PlaSimulatorTestProxy {
public:
    static void fireReactionsEuler(PlaSimulator& sim, std::vector<double>& state, const std::vector<double>& a,
                                   const std::vector<RxnClass>& classes, double tau, std::mt19937& rng) {
        sim.fireReactionsEuler(state, a, classes, tau, rng);
    }
    static void setFixedSpecies(PlaSimulator& sim, const std::vector<bool>& fixed) { sim.fixedSpecies_ = fixed; }
    static bool checkNegativePopulations(const PlaSimulator& sim, const std::vector<double>& state) {
        return sim.checkNegativePopulations(state);
    }
    static void setupDummyReactions(PlaSimulator& sim) {
        sim.compiledRxns_.clear();
        PlaSimulator::CompiledReaction rxn0;
        rxn0.rateConstant = 1.0; rxn0.statFactor = 1.0; rxn0.stoich = {{0, -1}, {1, 1}};
        sim.compiledRxns_.push_back(rxn0);
        PlaSimulator::CompiledReaction rxn1;
        rxn1.rateConstant = 1.0; rxn1.statFactor = 1.0; rxn1.stoich = {{1, -2}, {0, 1}};
        sim.compiledRxns_.push_back(rxn1);
        PlaSimulator::CompiledReaction rxn2;
        rxn2.rateConstant = 1.0; rxn2.statFactor = 1.0; rxn2.stoich = {{0, 1}};
        sim.compiledRxns_.push_back(rxn2);
        sim.fixedSpecies_ = {false, false};
        sim.nSpecies_ = 2;
    }

    static void setupDummyGroups(PlaSimulator& sim) {
        sim.compiledGroups_.clear();
        PlaSimulator::CompiledGroup g1;
        g1.name = "Group1";
        g1.entries = {{0, 2.0}, {1, 1.5}};
        sim.compiledGroups_.push_back(g1);

        PlaSimulator::CompiledGroup g2;
        g2.name = "Group2";
        g2.entries = {{1, 3.0}, {2, 0.5}};
        sim.compiledGroups_.push_back(g2);
    }

    static void updateObservables(const PlaSimulator& sim, const std::vector<double>& state, std::vector<double>& obsValues) {
        sim.updateObservables(state, obsValues);
    }
};
}
TEST_CASE("PlaSimulator::fireReactionsEuler", "[PlaSimulator]") {
    Model model;
    GeneratedNetwork network;
    PlaSimulator sim(model, network);
    bng::engine::PlaSimulatorTestProxy::setupDummyReactions(sim);
    std::mt19937 rng(42);
    SECTION("Deterministic") {
        std::vector<double> state = {100.0, 50.0};
        std::vector<double> a = {10.0, 4.0, 2.0};
        std::vector<RxnClass> classes = {RxnClass::Deterministic, RxnClass::Deterministic, RxnClass::Deterministic};
        bng::engine::PlaSimulatorTestProxy::fireReactionsEuler(sim, state, a, classes, 0.5, rng);
        REQUIRE_THAT(state[0], Catch::Matchers::WithinAbs(98.0, 1e-6));
        REQUIRE_THAT(state[1], Catch::Matchers::WithinAbs(51.0, 1e-6));
    }
    SECTION("ExactStochastic / Poisson") {
        std::vector<double> state = {100.0, 50.0};
        std::vector<double> a = {10.0, 0.0, 0.0};
        std::vector<RxnClass> classes = {RxnClass::ExactStochastic, RxnClass::ExactStochastic, RxnClass::ExactStochastic};
        bng::engine::PlaSimulatorTestProxy::fireReactionsEuler(sim, state, a, classes, 0.5, rng);
        REQUIRE_THAT(state[0] + state[1], Catch::Matchers::WithinAbs(150.0, 1e-6));
        REQUIRE(state[0] != 100.0);
    }
    SECTION("Langevin") {
        std::vector<double> state = {100.0, 50.0};
        std::vector<double> a = {100.0, 0.0, 0.0};
        std::vector<RxnClass> classes = {RxnClass::Langevin, RxnClass::Langevin, RxnClass::Langevin};
        bng::engine::PlaSimulatorTestProxy::fireReactionsEuler(sim, state, a, classes, 1.0, rng);
        REQUIRE_THAT(state[0] + state[1], Catch::Matchers::WithinAbs(150.0, 1e-6));
    }
    SECTION("Fixed species are not updated") {
        std::vector<double> state = {100.0, 50.0};
        std::vector<double> a = {10.0, 0.0, 0.0};
        std::vector<RxnClass> classes = {RxnClass::Deterministic, RxnClass::Deterministic, RxnClass::Deterministic};
        bng::engine::PlaSimulatorTestProxy::setFixedSpecies(sim, {false, true});
        bng::engine::PlaSimulatorTestProxy::fireReactionsEuler(sim, state, a, classes, 0.5, rng);
        REQUIRE_THAT(state[0], Catch::Matchers::WithinAbs(95.0, 1e-6));
        REQUIRE_THAT(state[1], Catch::Matchers::WithinAbs(50.0, 1e-6));
    }
    SECTION("Zero propensity skips reaction") {
        std::vector<double> state = {100.0, 50.0};
        std::vector<double> a = {0.0, 0.0, 0.0};
        std::vector<RxnClass> classes = {RxnClass::Deterministic, RxnClass::Deterministic, RxnClass::Deterministic};
        bng::engine::PlaSimulatorTestProxy::fireReactionsEuler(sim, state, a, classes, 1.0, rng);
        REQUIRE_THAT(state[0], Catch::Matchers::WithinAbs(100.0, 1e-6));
        REQUIRE_THAT(state[1], Catch::Matchers::WithinAbs(50.0, 1e-6));
    }
}

TEST_CASE("PlaSimulator::checkNegativePopulations", "[PlaSimulator]") {
    Model model;
    GeneratedNetwork network;
    PlaSimulator sim(model, network);

    // Setup dummy reactions to initialize fixedSpecies_ and nSpecies_
    bng::engine::PlaSimulatorTestProxy::setupDummyReactions(sim);

    SECTION("All positive populations") {
        std::vector<double> state = {100.0, 50.0};
        REQUIRE_FALSE(bng::engine::PlaSimulatorTestProxy::checkNegativePopulations(sim, state));
    }

    SECTION("Negative but > -0.5") {
        std::vector<double> state = {-0.3, 50.0};
        REQUIRE_FALSE(bng::engine::PlaSimulatorTestProxy::checkNegativePopulations(sim, state));
    }

    SECTION("Negative < -0.5") {
        std::vector<double> state = {-0.6, 50.0};
        REQUIRE(bng::engine::PlaSimulatorTestProxy::checkNegativePopulations(sim, state));
    }

    SECTION("Fixed species < -0.5 are ignored") {
        bng::engine::PlaSimulatorTestProxy::setFixedSpecies(sim, {true, false});
        std::vector<double> state = {-0.6, 50.0};
        REQUIRE_FALSE(bng::engine::PlaSimulatorTestProxy::checkNegativePopulations(sim, state));
    }
}

TEST_CASE("PlaSimulator::updateObservables", "[PlaSimulator]") {
    Model model;
    GeneratedNetwork network;
    PlaSimulator sim(model, network);

    bng::engine::PlaSimulatorTestProxy::setupDummyGroups(sim);

    SECTION("Calculates observables correctly") {
        std::vector<double> state = {10.0, 5.0, 2.0};
        std::vector<double> obsValues;

        bng::engine::PlaSimulatorTestProxy::updateObservables(sim, state, obsValues);

        REQUIRE(obsValues.size() == 2);
        // Group 1: 10.0 * 2.0 + 5.0 * 1.5 = 20.0 + 7.5 = 27.5
        REQUIRE_THAT(obsValues[0], Catch::Matchers::WithinAbs(27.5, 1e-6));
        // Group 2: 5.0 * 3.0 + 2.0 * 0.5 = 15.0 + 1.0 = 16.0
        REQUIRE_THAT(obsValues[1], Catch::Matchers::WithinAbs(16.0, 1e-6));
    }

    SECTION("Empty state handles gracefully") {
        std::vector<double> state = {0.0, 0.0, 0.0};
        std::vector<double> obsValues;

        bng::engine::PlaSimulatorTestProxy::updateObservables(sim, state, obsValues);

        REQUIRE(obsValues.size() == 2);
        REQUIRE_THAT(obsValues[0], Catch::Matchers::WithinAbs(0.0, 1e-6));
        REQUIRE_THAT(obsValues[1], Catch::Matchers::WithinAbs(0.0, 1e-6));
    }
}
