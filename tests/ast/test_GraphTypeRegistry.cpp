#include <catch2/catch_test_macros.hpp>
#include "GraphTypeRegistry.hpp"
#include "MoleculeType.hpp"
#include "core/BNGcore.hpp"

using namespace bng::ast;

TEST_CASE("GraphTypeRegistry functionality", "[ast]") {
    GraphTypeRegistry registry;

    SECTION("ensureMoleculeType - caching behavior") {
        MoleculeType m("A", {});
        const BNGcore::EntityType& first = registry.ensureMoleculeType(m);
        const BNGcore::EntityType& second = registry.ensureMoleculeType(m);

        REQUIRE(&first == &second);
        REQUIRE(first.get_type_name() == "A");
    }

    SECTION("ensureMoleculeType - with components") {
        ComponentType c1{"x", {"P", "U"}};
        ComponentType c2{"y", {}};
        MoleculeType m("A", {c1, c2, c1}); // notice c1 appears twice

        const BNGcore::EntityType& mNodeType = registry.ensureMoleculeType(m);

        REQUIRE(mNodeType.get_type_name() == "A");

        // Out edges represent the components. Since c1 is there twice and c2 is there once,
        // it should have edges to the 'x' component type (multiplicity 2) and 'y' (multiplicity 1).
        // Let's verify we have 2 distinct outgoing edge types.
        auto out_begin = mNodeType.edges_out_begin();
        auto out_end = mNodeType.edges_out_end();
        int edgeTypeCount = 0;
        int totalEdges = 0;
        for(auto it = out_begin; it != out_end; ++it) {
            edgeTypeCount++;
            // Note: `it->second` is a NodeFunction*. If it's a ConstantNodeFunction, we can't easily query its value directly without a Node,
            // but we can map a dummy node or we just test we have the right number of edge types.
            // The mapping multiplicity by component ensures 2 types here.
        }
        REQUIRE(edgeTypeCount == 2);
    }

    SECTION("ensureComponentType - caching and links") {
        ComponentType c1{"x", {"P", "U"}};
        MoleculeType m("A", {c1});

        const BNGcore::EntityType& cNode1 = registry.ensureComponentType(m, c1);
        const BNGcore::EntityType& cNode2 = registry.ensureComponentType(m, c1);

        REQUIRE(&cNode1 == &cNode2);
        REQUIRE(cNode1.get_type_name() == "x");

        // Component has out-edge to BOND_NODE_TYPE
        auto out_begin = cNode1.edges_out_begin();
        auto out_end = cNode1.edges_out_end();
        bool hasBondEdge = false;
        for(auto it = out_begin; it != out_end; ++it) {
            if (it->first->get_type_name() == BNGcore::BOND_NODE_TYPE.get_type_name()) {
                hasBondEdge = true;
            }
        }
        REQUIRE(hasBondEdge);

        // State validation
        const BNGcore::StateType& stateType = cNode1.get_state_type();
        REQUIRE(stateType.get_label() == "A::x");
    }

    SECTION("mergeFrom") {
        GraphTypeRegistry otherRegistry;
        MoleculeType mA("A", {});
        MoleculeType mB("B", {});

        const BNGcore::EntityType& firstA = registry.ensureMoleculeType(mA);
        const BNGcore::EntityType& otherB = otherRegistry.ensureMoleculeType(mB);

        // A is already in registry, so merging mA should keep ours.
        // It should also pull in mB.
        const BNGcore::EntityType& otherA = otherRegistry.ensureMoleculeType(mA);

        registry.mergeFrom(otherRegistry);

        const BNGcore::EntityType& ensureA_after = registry.ensureMoleculeType(mA);
        const BNGcore::EntityType& ensureB_after = registry.ensureMoleculeType(mB);

        REQUIRE(&ensureA_after == &firstA);
        REQUIRE(&ensureB_after == &otherB);
    }
}
