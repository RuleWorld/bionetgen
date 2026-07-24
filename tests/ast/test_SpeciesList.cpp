#include <catch2/catch_test_macros.hpp>
#include "ast/SpeciesList.hpp"
#include "ast/SpeciesGraph.hpp"
#include "core/BNGcore.hpp"

using namespace bng::ast;

TEST_CASE("SpeciesList functionality", "[ast][SpeciesList]") {
    BNGcore::PatternGraph pg;
    SpeciesGraph sg(pg);
    Species s1(sg);

    SECTION("checkIso disabled allows duplicate additions") {
        SpeciesList list;

        // Turn off isomorphism checking
        list.setCheckIso(false);
        REQUIRE(list.getCheckIso() == false);

        // Add the same species twice
        list.add(s1);
        list.add(s1);

        // Since checkIso is false, both should be added unconditionally
        // rather than deduped.
        REQUIRE(list.size() == 2);
    }

    SECTION("checkIso enabled prevents duplicate additions") {
        SpeciesList list;

        // Isomorphism checking should be true by default
        REQUIRE(list.getCheckIso() == true);

        // Add the same species twice
        list.add(s1);
        list.add(s1);

        // Since checkIso is true, the second addition should be deduped
        REQUIRE(list.size() == 1);
    }
}
