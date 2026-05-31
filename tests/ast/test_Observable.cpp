#include <catch2/catch_test_macros.hpp>
#include "Observable.hpp"

using namespace bng::ast;

TEST_CASE("Observable functionality", "[ast]") {

    SECTION("creation and getters with typical values") {
        std::vector<std::string> patterns = {"A()", "B()"};
        Observable obs("MyObs", "Molecules", patterns);

        REQUIRE(obs.getName() == "MyObs");
        REQUIRE(obs.getType() == "Molecules");

        const auto& p = obs.getPatterns();
        REQUIRE(p.size() == 2);
        REQUIRE(p[0] == "A()");
        REQUIRE(p[1] == "B()");
    }

    SECTION("creation with empty patterns") {
        std::vector<std::string> empty_patterns;
        Observable obs("EmptyObs", "Species", empty_patterns);

        REQUIRE(obs.getName() == "EmptyObs");
        REQUIRE(obs.getType() == "Species");
        REQUIRE(obs.getPatterns().empty());
    }
}
