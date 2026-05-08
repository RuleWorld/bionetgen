#include <catch2/catch_test_macros.hpp>
#include "BNGcore.hpp"

using namespace BNGcore;

TEST_CASE("Node::operator==", "[BNGcore][Node]") {
    SECTION("Nodes with same type and state are equal") {
        NodeType type("A", ENTITY_NODE_TYPE, NULL_STATE_TYPE);
        Node node1(type);
        Node node2(type);
        REQUIRE(node1 == node2);
    }

    SECTION("Nodes with different types are not equal") {
        NodeType type1("A", ENTITY_NODE_TYPE, NULL_STATE_TYPE);
        NodeType type2("B", ENTITY_NODE_TYPE, NULL_STATE_TYPE);
        Node node1(type1);
        Node node2(type2);
        REQUIRE_FALSE(node1 == node2);
    }

    SECTION("Nodes with different states are not equal") {
        LabelStateType l_state_type("l_state", "state1");
        l_state_type.add_state("state2");
        NodeType type("A", ENTITY_NODE_TYPE, l_state_type);

        Node node1(type);
        Node node2(type);

        State* state1 = l_state_type.get_state_object();
        state1->set_state("state1");

        State* state2 = l_state_type.get_state_object();
        state2->set_state("state2");

        node1.set_state(*state1);
        node2.set_state(*state2);

        REQUIRE_FALSE(node1 == node2);

        delete state1;
        delete state2;
    }

    SECTION("Nodes with wildcard states match any state") {
        LabelStateType l_state_type("l_state", "state1");
        l_state_type.add_state("state2");
        NodeType type("A", ENTITY_NODE_TYPE, l_state_type);

        Node node1(type);
        Node node2(type);

        State* state_wildcard = l_state_type.get_state_object();
        state_wildcard->set_state(WILDCARD_STRING);

        State* state_specific = l_state_type.get_state_object();
        state_specific->set_state("state2");

        node1.set_state(*state_wildcard);
        node2.set_state(*state_specific);

        REQUIRE(node1 == node2);
        REQUIRE(node2 == node1); // symmetry check for wildcard

        delete state_wildcard;
        delete state_specific;
    }

    SECTION("Compartment matching") {
        NodeType type("A", ENTITY_NODE_TYPE, NULL_STATE_TYPE);
        Node node1(type);
        Node node2(type);

        // Empty compartment on pattern (node1) acts as wildcard
        node2.set_compartment("comp1");
        REQUIRE(node1 == node2);

        // Same compartment
        node1.set_compartment("comp1");
        REQUIRE(node1 == node2);

        // Different compartment
        node2.set_compartment("comp2");
        REQUIRE_FALSE(node1 == node2);

        // Pattern specifies compartment, target does not
        node2.set_compartment("");
        REQUIRE_FALSE(node1 == node2);
    }
}
