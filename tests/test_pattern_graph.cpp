#include <catch2/catch_test_macros.hpp>
#include "BNGcore.hpp"

using namespace BNGcore;

TEST_CASE("PatternGraph operations", "[BNGcore][PatternGraph]") {
    SECTION("Empty graph creation") {
        PatternGraph graph;
        REQUIRE(graph.size() == 0);
        REQUIRE(graph.empty() == true);
        REQUIRE(graph.is_canonical() == false);
    }

    SECTION("Node addition and removal") {
        PatternGraph graph;
        NodeType type("A", ENTITY_NODE_TYPE, NULL_STATE_TYPE);
        Node node1(type);
        Node* added_node = graph.add_node(node1);

        REQUIRE(graph.size() == 1);
        REQUIRE(graph.empty() == false);
        REQUIRE(added_node != nullptr);
        REQUIRE(added_node->get_type().get_type_name() == "A");

        // Remove node
        bool deleted = graph.delete_node(added_node);
        REQUIRE(deleted == true);
        REQUIRE(graph.size() == 0);
        REQUIRE(graph.empty() == true);

        // Try deleting again
        bool deleted_again = graph.delete_node(added_node);
        REQUIRE(deleted_again == false);
    }

    SECTION("Edge addition and removal") {
        PatternGraph graph;
        NodeType typeA("A", ENTITY_NODE_TYPE, NULL_STATE_TYPE);
        NodeType typeB("B", ENTITY_NODE_TYPE, NULL_STATE_TYPE);

        Node nA(typeA);
        Node nB(typeB);
        Node* nodeA = graph.add_node(nA);
        Node* nodeB = graph.add_node(nB);

        // Add edge
        bool added = graph.add_edge(nodeA, nodeB);
        REQUIRE(added == true);

        // Verify edge exists
        bool found = false;
        for (auto it = nodeA->edges_out_begin(); it != nodeA->edges_out_end(); ++it) {
            if (*it == nodeB) found = true;
        }
        REQUIRE(found == true);

        // Remove edge
        bool deleted = graph.delete_edge(nodeA, nodeB);
        REQUIRE(deleted == true);

        found = false;
        for (auto it = nodeA->edges_out_begin(); it != nodeA->edges_out_end(); ++it) {
            if (*it == nodeB) found = true;
        }
        REQUIRE(found == false);
    }

    SECTION("Graph stringification (get_BNG2_string)") {
        PatternGraph graph;
        NodeType typeA("A", ENTITY_NODE_TYPE, NULL_STATE_TYPE);
        NodeType typeB("B", ENTITY_NODE_TYPE, NULL_STATE_TYPE);
        Node nA(typeA);
        Node nB(typeB);
        Node* nodeA = graph.add_node(nA);
        Node* nodeB = graph.add_node(nB);
        graph.add_edge(nodeA, nodeB);

        std::string bng_str = graph.get_BNG2_string();
        REQUIRE(bng_str.length() > 0);
    }

    SECTION("Splicing two PatternGraphs") {
        PatternGraph graph1;
        PatternGraph graph2;

        NodeType typeA("A", ENTITY_NODE_TYPE, NULL_STATE_TYPE);
        Node n1(typeA);
        Node n2(typeA);

        graph1.add_node(n1);
        graph2.add_node(n2);

        Map overlap_map; // empty overlap map means disjoint union
        bool spliced = graph1.splice(graph2, overlap_map);
        REQUIRE(spliced == true);
        REQUIRE(graph1.size() == 2);
    }

    SECTION("Copy and move constructor") {
        PatternGraph graph1;
        NodeType typeA("A", ENTITY_NODE_TYPE, NULL_STATE_TYPE);
        Node n1(typeA);
        graph1.add_node(n1);

        PatternGraph graph2(graph1);
        REQUIRE(graph2.size() == 1);
        REQUIRE((*graph2.begin())->get_type().get_type_name() == "A");

        PatternGraph graph3(std::move(graph1));
        REQUIRE(graph3.size() == 1);
        REQUIRE((*graph3.begin())->get_type().get_type_name() == "A");
    }

    SECTION("validate") {
        PatternGraph graph;
        NodeType type("A", ENTITY_NODE_TYPE, NULL_STATE_TYPE);
        Node node1(type);
        graph.add_node(node1);

        bool valid = graph.validate(false);
        REQUIRE(valid == true); // Should be valid
    }
}

TEST_CASE("PatternGraph component splitting", "[BNGcore][PatternGraph]") {
    SECTION("Splitting connected components") {
        PatternGraph graph;
        NodeType typeA("A", ENTITY_NODE_TYPE, NULL_STATE_TYPE);
        NodeType typeB("B", ENTITY_NODE_TYPE, NULL_STATE_TYPE);

        Node n1(typeA);
        Node n2(typeA);
        Node n3(typeB);
        Node* node1 = graph.add_node(n1);
        Node* node2 = graph.add_node(n2);
        Node* node3 = graph.add_node(n3);

        // connect 1 and 2
        graph.add_edge(node1, node2);

        patterngraph_container_t split_graphs;
        graph.split_connected(split_graphs);

        // We expect two graphs: one with nodes 1 & 2, and another with node 3
        REQUIRE(split_graphs.size() == 2);
    }
}

TEST_CASE("PatternGraph mapping and subtrees", "[BNGcore][PatternGraph]") {
    SECTION("gather_subtree functionality") {
        PatternGraph graph;
        NodeType typeM("M", ENTITY_NODE_TYPE, NULL_STATE_TYPE);
        NodeType typeC("c", COMPONENT_NODE_TYPE, NULL_STATE_TYPE);

        Node nM(typeM);
        Node nC1(typeC);
        Node nC2(typeC);
        Node* mol = graph.add_node(nM);
        Node* comp1 = graph.add_node(nC1);
        Node* comp2 = graph.add_node(nC2);

        graph.add_edge(mol, comp1);
        graph.add_edge(mol, comp2);

        node_container_t visited;
        node_container_t bonds;

        // start gather from molecule
        graph.gather_subtree(mol, visited, bonds);

        REQUIRE(visited.size() == 3);
        REQUIRE(bonds.size() == 0); // They are internal subtree edges, not inter-molecule bonds
    }
}

TEST_CASE("PatternGraph embedding check", "[BNGcore][PatternGraph]") {
    SECTION("Valid embedding returns true") {
        PatternGraph target;
        NodeType typeA("A", ENTITY_NODE_TYPE, NULL_STATE_TYPE);
        Node tA(typeA);
        target.add_node(tA);

        PatternGraph query;
        Node qA(typeA);
        query.add_node(qA);

        Map overlap(&query, &target);
        overlap.insert(*(query.begin()), *(target.begin()));

        REQUIRE(query.check_embedding(overlap) == true);
    }
}

TEST_CASE("PatternGraph canonical finding and fast merge", "[BNGcore][PatternGraph]") {
    SECTION("Finding canonical order sets canonical_flag to true") {
        PatternGraph graph;
        NodeType type("A", ENTITY_NODE_TYPE, NULL_STATE_TYPE);
        Node node1(type);
        graph.add_node(node1);

        REQUIRE(graph.is_canonical() == false);

        std::string label = graph.get_label();
        REQUIRE(graph.is_canonical() == true);
        REQUIRE(label.length() > 0);

        // After finding canonical order (done internally via get_label or explicit calls)
        // Adding a node should unset it
        graph.add_node(node1);
        REQUIRE(graph.is_canonical() == false);
    }

    SECTION("quick_merge merges node collections") {
        PatternGraph graph1;
        PatternGraph graph2;
        NodeType type("A", ENTITY_NODE_TYPE, NULL_STATE_TYPE);
        Node n1(type);
        Node n2(type);
        graph1.add_node(n1);
        graph2.add_node(n2);

        patterngraph_container_t to_merge;
        to_merge.push_back(graph2);

        graph1.quick_merge(to_merge);
        // Size should be 2 after merging graph2's nodes
        REQUIRE(graph1.size() == 2);
    }
}

TEST_CASE("PatternGraph complex structures and validation", "[BNGcore][PatternGraph]") {
    SECTION("Contains instance nodes check") {
        PatternGraph graph;
        NodeType typeA("A", ENTITY_NODE_TYPE, NULL_STATE_TYPE);

        Node node1(typeA);
        graph.add_node(node1);

        bool contains_instance = graph.contains_instance_nodes();
        REQUIRE(contains_instance == false);

        // Add an instance node
        NodeType typeInst("Inst", ENTITY_NODE_TYPE, NULL_STATE_TYPE);
        typeInst.set_instance_flag(true);
        Node node2(typeInst);
        graph.add_node(node2);

        REQUIRE(graph.contains_instance_nodes() == true);
    }
}

TEST_CASE("PatternGraph printing and reset_index", "[BNGcore][PatternGraph]") {
    SECTION("reset_index correctly assigns indices") {
        PatternGraph graph;
        NodeType typeA("A", ENTITY_NODE_TYPE, NULL_STATE_TYPE);

        Node n1(typeA);
        Node n2(typeA);

        Node* node1 = graph.add_node(n1);
        Node* node2 = graph.add_node(n2);

        node1->set_index(999);
        node2->set_index(999);

        graph.reset_index();

        // Node indices should now be reset to -1 according to BNGcore's reset_index
        REQUIRE(node1->get_index() == -1);
        REQUIRE(node2->get_index() == -1);
        REQUIRE(graph.is_canonical() == false);
    }
}
