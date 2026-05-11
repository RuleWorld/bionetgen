#include <catch2/catch_test_macros.hpp>
#include "Model.hpp"
#include "Compartment.hpp"
#include "Molecule.hpp"
#include "Parameter.hpp"
#include "Function.hpp"
#include "EnergyPattern.hpp"
#include "Observable.hpp"
#include "MoleculeType.hpp"
#include "SeedSpecies.hpp"
#include "ReactionRule.hpp"
#include "PopulationMap.hpp"
#include "Expression.hpp"

using namespace bng::ast;

TEST_CASE("Model attributes getters and setters", "[ast][Model]") {
    Model model;

    SECTION("Version, SubstanceUnits, ModelName, Options") {
        model.setVersion("1.0.0");
        REQUIRE(model.getVersion() == "1.0.0");

        model.setSubstanceUnits("molecules");
        REQUIRE(model.getSubstanceUnits() == "molecules");

        model.setModelName("TestModel");
        REQUIRE(model.getModelName() == "TestModel");

        model.setOption("option1", "value1");
        model.setOption("option2", "value2");
        auto options = model.getOptions();
        REQUIRE(options.size() == 2);
        REQUIRE(options["option1"] == "value1");
        REQUIRE(options["option2"] == "value2");
    }

    SECTION("Compartments") {
        Compartment comp("cell", 1.0);
        model.addCompartment(comp);
        REQUIRE(model.getCompartments().size() == 1);
        REQUIRE(model.getCompartments()[0].getName() == "cell");
    }

    SECTION("Molecules") {
        Molecule mol("A");
        model.addMolecule(mol);
        REQUIRE(model.getMolecules().size() == 1);
        REQUIRE(model.getMolecules()[0].getName() == "A");
    }

    SECTION("Parameters") {
        Parameter param("k", Expression());
        model.addParameter(param);
        REQUIRE(model.getParameters().all().size() == 1);
        REQUIRE(model.getParameters().all()[0].getName() == "k");
    }

    SECTION("Actions") {
        Action act;
        act.name = "simulate";
        act.arguments["method"] = "ode";
        model.addAction(act);
        REQUIRE(model.getActions().size() == 1);
        REQUIRE(model.getActions()[0].name == "simulate");
        REQUIRE(model.getActions()[0].arguments["method"] == "ode");
    }

    SECTION("Functions") {
        Function func("f", {}, Expression());
        model.addFunction(func);
        REQUIRE(model.getFunctions().size() == 1);
        REQUIRE(model.getFunctions()[0].getName() == "f");
    }

    SECTION("EnergyPatterns") {
        EnergyPattern ep("ep1", "", Expression(), SpeciesGraph());
        model.addEnergyPattern(ep);
        REQUIRE(model.getEnergyPatterns().size() == 1);
        REQUIRE(model.getEnergyPatterns()[0].getLabel() == "ep1");
    }

    SECTION("Observables") {
        Observable obs("obs1", "Molecules", {});
        model.addObservable(obs);
        REQUIRE(model.getObservables().size() == 1);
        REQUIRE(model.getObservables()[0].getName() == "obs1");
    }

    SECTION("MoleculeTypes") {
        MoleculeType mt("A", {});
        model.addMoleculeType(mt);
        REQUIRE(model.getMoleculeTypes().size() == 1);
        REQUIRE(model.getMoleculeTypes()[0].getName() == "A");
    }

    SECTION("SeedSpecies") {
        SeedSpecies ss("S", Expression());
        model.addSeedSpecies(ss);
        REQUIRE(model.getSeedSpecies().size() == 1);
        REQUIRE(model.getSeedSpecies()[0].getPattern() == "S");
    }

    SECTION("ReactionRules") {
        ReactionRule rr("rule1", "", {}, {}, {}, {}, false, {}, {});
        model.addReactionRule(std::move(rr));
        REQUIRE(model.getReactionRules().size() == 1);
        REQUIRE(model.getReactionRules()[0].getRuleName() == "rule1");
    }

    SECTION("PopulationMaps") {
        PopulationMap pm;
        pm.populationFunction = "map1";
        model.addPopulationMap(pm);
        REQUIRE(model.getPopulationMaps().size() == 1);
        REQUIRE(model.getPopulationMaps()[0].populationFunction == "map1");
    }
}

TEST_CASE("Model merge", "[ast][Model]") {
    Model source;
    Model dest;

    // Add elements to source
    source.setVersion("2.0.0");
    source.setSubstanceUnits("moles");
    source.setModelName("SourceModel");
    source.setOption("key", "value");

    Compartment comp("cell", 1.0);
    source.addCompartment(comp);

    Parameter param("k", Expression());
    source.addParameter(param);

    Action act;
    act.name = "simulate";
    source.addAction(act);

    MoleculeType mt("A", {});
    source.addMoleculeType(mt);

    ReactionRule rr("rule1", "", {}, {}, {}, {}, false, {}, {});
    source.addReactionRule(std::move(rr));

    // Merge source into dest
    dest.merge(source);

    // Verify merged elements
    REQUIRE(dest.getCompartments().size() == 1);
    REQUIRE(dest.getCompartments()[0].getName() == "cell");

    REQUIRE(dest.getParameters().all().size() == 1);
    REQUIRE(dest.getParameters().all()[0].getName() == "k");

    REQUIRE(dest.getMoleculeTypes().size() == 1);
    REQUIRE(dest.getMoleculeTypes()[0].getName() == "A");

    REQUIRE(dest.getReactionRules().size() == 1);
    REQUIRE(dest.getReactionRules()[0].getRuleName() == "rule1");

    REQUIRE(dest.getOptions().size() == 1);
    REQUIRE(dest.getOptions().at("key") == "value");

    // Verify actions are NOT merged
    REQUIRE(dest.getActions().size() == 0);
}
