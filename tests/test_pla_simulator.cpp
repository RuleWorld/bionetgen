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
