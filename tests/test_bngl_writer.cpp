#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "../src/io/BnglWriter.hpp"
#include "../src/ast/Model.hpp"
#include <cmath>

using namespace bng::io;
using namespace bng::ast;

TEST_CASE("BnglWriter basic serialization", "[BnglWriter]") {
    Model model;
    model.setModelName("TestModel");

    Parameter p1("k1", Expression::number(1.5));
    p1.setValue(1.5);
    model.addParameter(p1);

    Parameter p2("large", Expression::number(1e8));
    p2.setValue(1e8);
    model.addParameter(p2);

    Parameter p3("small", Expression::number(1e-5));
    p3.setValue(1e-5);
    model.addParameter(p3);

    Parameter p4("inf_val", Expression::number(INFINITY));
    p4.setValue(INFINITY);
    model.addParameter(p4);

    Compartment comp("cell", 3, 1.0);
    model.addCompartment(comp);

    MoleculeType mt("A", {});
    model.addMoleculeType(std::move(mt));

    MoleculeType mt2("B", {ComponentType{"p", {"U", "P"}}});
    model.addMoleculeType(std::move(mt2));

    model.addSeedSpecies(SeedSpecies("A()", Expression::number(100)));

    model.addObservable(Observable("O1", "Species", {"A()"}));

    Function func("f1", {}, Expression::number(2.0));
    model.addFunction(std::move(func));

    ReactionRule rule("Rule1", "", {"A()"}, {"A()"}, {Expression::number(1.5)}, {}, false);
    model.addReactionRule(std::move(rule));

    Action action;
    action.name = "simulate";
    action.arguments.insert({"method", "ode"});
    action.arguments.insert({"t_end", "100"});
    model.addAction(std::move(action));

    BnglWriter::Options opts;
    opts.evaluateExpressions = true;
    opts.includeActions = true;
    opts.includeComments = true;
    std::string res = BnglWriter::write(model, nullptr, opts);

    REQUIRE(res.find("begin model") != std::string::npos);
    REQUIRE(res.find("k1 1.5") != std::string::npos);
    REQUIRE(res.find("cell 1 3") != std::string::npos);
    REQUIRE(res.find("A") != std::string::npos);
    REQUIRE(res.find("B(p~U~P)") != std::string::npos);
    REQUIRE(res.find("Species O1 A()") != std::string::npos);
    REQUIRE(res.find("f1() = 2") != std::string::npos);
    REQUIRE(res.find("Rule1: A() -> A() 1.5") != std::string::npos);
    REQUIRE(res.find("simulate(method=>\"ode\", t_end=>\"100\")") != std::string::npos);
    REQUIRE(res.find("end model") != std::string::npos);

    // Unevaluated expression test
    BnglWriter::Options opts2;
    opts2.evaluateExpressions = false;
    std::string res2 = BnglWriter::write(model, nullptr, opts2);
    REQUIRE(res2.find("k1 1.5") != std::string::npos);
}

TEST_CASE("BnglWriter number formatting", "[BnglWriter]") {
    Model model;

    // Very large number > 1e6
    Parameter p1("p1", Expression::number(1000001));
    p1.setValue(1000001);
    model.addParameter(p1);

    // Very small number < 1e-3
    Parameter p2("p2", Expression::number(0.0001));
    p2.setValue(0.0001);
    model.addParameter(p2);

    // NaN / Infinity check via formatNumber logic inside BnglWriter (it should yield "0")
    Parameter p3("p3", Expression::number(NAN));
    p3.setValue(NAN);
    model.addParameter(p3);

    BnglWriter::Options opts;
    opts.evaluateExpressions = true;
    std::string res = BnglWriter::write(model, nullptr, opts);

    REQUIRE(res.find("p1 1.000001e+06") != std::string::npos);
    REQUIRE(res.find("p2 1.000000e-04") != std::string::npos);
    REQUIRE(res.find("p3 0") != std::string::npos);
}
