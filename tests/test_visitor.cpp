#include <catch2/catch_test_macros.hpp>

#include "parser/BNGAstVisitor.hpp"

TEST_CASE("BNGAstVisitor builds parameters and actions", "[visitor]") {
    const std::string source = R"(begin model
begin parameters
  a 2
  b 3*a
  c if(b>5,b+1,0)
end parameters
begin compartments
  Cell 3 c
end compartments
begin actions
  generate_network({max_iter=>10,overwrite=>1})
  simulate({method=>"ode",t_end=>100,n_steps=>5})
  setParameter("a", 5)
end actions
end model
)";

    auto model = bng::parser::parseModel(source);

    REQUIRE(model->getParameters().size() == 3);
    REQUIRE(model->getParameters().get("a").getValue() == 2.0);
    REQUIRE(model->getParameters().get("b").getValue() == 6.0);
    REQUIRE(model->getParameters().get("c").getValue() == 7.0);

    REQUIRE(model->getActions().size() == 3);
    REQUIRE(model->getActions()[0].name == "generate_network");
    REQUIRE(model->getActions()[0].arguments.at("max_iter") == "10");
    REQUIRE(model->getActions()[1].name == "simulate");
    REQUIRE(model->getActions()[1].arguments.at("method") == "\"ode\"");
    REQUIRE(model->getActions()[2].name == "setparameter");
    REQUIRE(model->getActions()[2].arguments.at("target") == "a");
    REQUIRE(model->getActions()[2].arguments.at("value") == "5");
    REQUIRE(model->getCompartments().size() == 1);
    REQUIRE(model->getCompartments()[0].getName() == "Cell");
    REQUIRE(model->getCompartments()[0].getDimension() == 3);
    REQUIRE(model->getCompartments()[0].getVolume() == 7.0);
}

TEST_CASE("BNGAstVisitor captures semantic model blocks", "[visitor][blocks]") {
    const std::string source = R"(begin model
begin parameters
  V 1
  k1 2*V
end parameters
begin molecule types
  A(x~u~p,y)
  B(z)
end molecule types
begin seed species
  A(x~u,y) 100
  B(z) 50
end seed species
begin observables
  Molecules Atot A(x~u,y)
end observables
begin reaction rules
  bind: A(x~u,y)+B(z) -> A(x~p,y).B(z) k1 DeleteMolecules
end reaction rules
begin actions
  generate_network({max_iter=>10})
end actions
end model
)";

    auto model = bng::parser::parseModel(source);

    REQUIRE(model->getMoleculeTypes().size() == 2);
    REQUIRE(model->getMoleculeTypes()[0].getName() == "A");
    REQUIRE(model->getMoleculeTypes()[0].getComponents().size() == 2);
    REQUIRE(model->getMoleculeTypes()[0].getComponents()[0].allowedStates.size() == 2);

    REQUIRE(model->getSeedSpecies().size() == 2);
    REQUIRE(model->getSeedSpecies()[0].getPattern() == "A(x~u,y)");
    REQUIRE(model->getSeedSpecies()[0].getGraph().get_BNG2_string() == "A(x~u,y)");
    REQUIRE(!model->getSeedSpecies()[0].getCanonicalLabel().empty());

    REQUIRE(model->getReactionRules().size() == 1);
    REQUIRE(model->getReactionRules()[0].getLabel() == "bind:");
    REQUIRE(model->getReactionRules()[0].getReactants().size() == 2);
    REQUIRE(model->getReactionRules()[0].getProducts().size() == 1);
    REQUIRE(model->getReactionRules()[0].getRates().size() == 1);
    REQUIRE(model->getReactionRules()[0].getModifiers().size() == 1);
    REQUIRE(model->getReactionRules()[0].getModifiers()[0] == "DeleteMolecules");
}

TEST_CASE("Seed species bridge constructs bound graphs", "[visitor][graphs]") {
    const std::string source = R"(begin model
begin molecule types
  A(x)
  B(y)
end molecule types
begin seed species
  A(x!1).B(y!1) 1
end seed species
end model
)";

    auto model = bng::parser::parseModel(source);

    REQUIRE(model->getSeedSpecies().size() == 1);
    REQUIRE(model->getSeedSpecies()[0].getGraph().get_BNG2_string() == "A(x!1).B(y!1)");
}

TEST_CASE("Seed species preserves trailing compartment names", "[visitor][graphs]") {
    const std::string source = R"(begin model
begin compartments
  C 3 1
end compartments
begin molecule types
  A(x)
end molecule types
begin seed species
  A(x)@C 1
end seed species
end model
)";

    auto model = bng::parser::parseModel(source);

    REQUIRE(model->getSeedSpecies().size() == 1);
    REQUIRE(model->getSeedSpecies()[0].getCompartment() == "C");
}

TEST_CASE("BNGAstVisitor builds functions and observables", "[visitor]") {
    const std::string source = R"(begin model
begin functions
  f(x) x+1
end functions
begin energy patterns
  ep1: A(b) x+2
end energy patterns
begin observables
  Molecules Obs A(b)
end observables
end model
)";

    auto model = bng::parser::parseModel(source);

    REQUIRE(model->getFunctions().size() == 1);
    REQUIRE(model->getFunctions()[0].getName() == "f");
    REQUIRE(model->getFunctions()[0].getArgs().size() == 1);
    REQUIRE(model->getFunctions()[0].getArgs()[0] == "x");

    REQUIRE(model->getEnergyPatterns().size() == 1);
    REQUIRE(model->getEnergyPatterns()[0].getLabel() == "ep1");
    REQUIRE(model->getEnergyPatterns()[0].getPattern() == "A(b)");

    REQUIRE(model->getObservables().size() == 1);
    REQUIRE(model->getObservables()[0].getName() == "Obs");
    REQUIRE(model->getObservables()[0].getType() == "Molecules");
    REQUIRE(model->getObservables()[0].getPatterns().size() == 1);
    REQUIRE(model->getObservables()[0].getPatterns()[0] == "A(b)");
}
