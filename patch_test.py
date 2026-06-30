import re

with open('tests/test_network_generator.cpp', 'r') as f:
    content = f.read()

test_case_code = """
TEST_CASE("parseCheckIso behaves correctly", "[NetworkGenerator]") {
    bng::ast::Model model;

    SECTION("returns true by default (no actions)") {
        REQUIRE(parseCheckIso(model) == true);
    }

    SECTION("returns true if no generate_network action exists") {
        bng::ast::Action action;
        action.name = "simulate";
        action.arguments["method"] = "ode";
        model.addAction(action);
        REQUIRE(parseCheckIso(model) == true);
    }

    SECTION("returns true if generate_network exists but no check_iso argument") {
        bng::ast::Action action;
        action.name = "generate_network";
        action.arguments["print_iter"] = "1";
        model.addAction(action);
        REQUIRE(parseCheckIso(model) == true);
    }

    SECTION("returns true if check_iso is set to true") {
        bng::ast::Action action;
        action.name = "generate_network";
        action.arguments["check_iso"] = "1";
        model.addAction(action);
        REQUIRE(parseCheckIso(model) == true);
    }

    SECTION("returns false if check_iso is set to false") {
        bng::ast::Action action;
        action.name = "generate_network";
        action.arguments["check_iso"] = "0";
        model.addAction(action);
        REQUIRE(parseCheckIso(model) == false);
    }

    SECTION("ignores generate_network without check_iso and finds the one with check_iso") {
        bng::ast::Action action1;
        action1.name = "generate_network";
        action1.arguments["print_iter"] = "1";
        model.addAction(action1);

        bng::ast::Action action2;
        action2.name = "generate_network";
        action2.arguments["check_iso"] = "0";
        model.addAction(action2);

        REQUIRE(parseCheckIso(model) == false);
    }

    SECTION("handles other boolean-like value 'yes' for check_iso") {
        bng::ast::Action action;
        action.name = "generate_network";
        action.arguments["check_iso"] = "yes";
        model.addAction(action);
        REQUIRE(parseCheckIso(model) == true);
    }

    SECTION("handles other boolean-like value 'no' for check_iso") {
        bng::ast::Action action;
        action.name = "generate_network";
        action.arguments["check_iso"] = "no";
        model.addAction(action);
        REQUIRE(parseCheckIso(model) == false);
    }
}
"""

with open('tests/test_network_generator.cpp', 'w') as f:
    f.write(content.replace('TEST_CASE("parseOverwrite behaves correctly", "[NetworkGenerator]") {', test_case_code + '\nTEST_CASE("parseOverwrite behaves correctly", "[NetworkGenerator]") {'))
