#include <catch2/catch_test_macros.hpp>
#include "BNGcore.hpp"

using namespace BNGcore;

TEST_CASE("Map operations", "[BNGcore][Map]") {
    NodeType typeA("A", ENTITY_NODE_TYPE, NULL_STATE_TYPE);
    NodeType typeB("B", ENTITY_NODE_TYPE, NULL_STATE_TYPE);

    PatternGraph source;
    Node nodeA1(typeA);
    source.add_node(nodeA1);

    PatternGraph target;
    Node nodeA2(typeA);
    target.add_node(nodeA2);

    SECTION("Empty map initialization") {
        Map map;
        REQUIRE(map.get_source_graph() == nullptr);
        REQUIRE(map.get_target_graph() == nullptr);
        REQUIRE(map.empty());
        REQUIRE(map.size() == 0);
    }

    SECTION("Map with source and target") {
        Map map(&source, &target);
        REQUIRE(map.get_source_graph() == &source);
        REQUIRE(map.get_target_graph() == &target);
        REQUIRE(map.empty());

        // Insert mapping
        REQUIRE(map.insert(*source.begin(), *target.begin()));
        REQUIRE_FALSE(map.empty());
        REQUIRE(map.size() == 1);

        // Check mapping
        Node* mapped = map.mapf(*source.begin());
        REQUIRE(mapped == *target.begin());

        // Find mapping
        auto iter = map.find(*source.begin());
        REQUIRE(iter != map.end());
        REQUIRE(iter->second == *target.begin());
    }

    SECTION("Automap") {
        Map map(source);
        REQUIRE(map.get_source_graph() == &source);
        REQUIRE(map.get_target_graph() == &source);
        REQUIRE(map.size() == 1);
        REQUIRE(map.mapf(*source.begin()) == *source.begin());
    }

    SECTION("Copy constructor") {
        Map map1(&source, &target);
        map1.insert(*source.begin(), *target.begin());

        Map map2(map1);
        REQUIRE(map2.get_source_graph() == &source);
        REQUIRE(map2.get_target_graph() == &target);
        REQUIRE(map2.size() == 1);
        REQUIRE(map2.mapf(*source.begin()) == *target.begin());
    }

    SECTION("Erase mapping") {
        Map map(&source, &target);
        map.insert(*source.begin(), *target.begin());
        REQUIRE(map.size() == 1);

        REQUIRE(map.erase(*source.begin()) == 1);
        REQUIRE(map.empty());
        REQUIRE(map.mapf(*source.begin()) == nullptr);
    }

    SECTION("Map composition (gof)") {
        PatternGraph graph1, graph2, graph3;

        Node n1(typeA); graph1.add_node(n1);
        Node n2(typeA); graph2.add_node(n2);
        Node n3(typeA); graph3.add_node(n3);

        Map f(&graph1, &graph2);
        f.insert(*graph1.begin(), *graph2.begin());

        Map g(&graph2, &graph3);
        g.insert(*graph2.begin(), *graph3.begin());

        Map* gof = g.mapf(f);
        REQUIRE(gof != nullptr);
        REQUIRE(gof->get_source_graph() == &graph1);
        REQUIRE(gof->get_target_graph() == &graph3);
        REQUIRE(gof->size() == 1);
        REQUIRE(gof->mapf(*graph1.begin()) == *graph3.begin());
        delete gof;
    }

    SECTION("Map composition with invalid targets returns null") {
        PatternGraph graph1, graph2, graph3;

        Map f(&graph1, &graph2);
        Map g(&graph1, &graph3); // Different source than f's target

        Map* gof = g.mapf(f);
        REQUIRE(gof == nullptr);
    }

    SECTION("Map invert") {
        Map map(&source, &target);
        map.insert(*source.begin(), *target.begin());

        Map* inverse = map.invert();
        REQUIRE(inverse != nullptr);
        REQUIRE(inverse->get_source_graph() == &target);
        REQUIRE(inverse->get_target_graph() == &source);
        REQUIRE(inverse->size() == 1);
        REQUIRE(inverse->mapf(*target.begin()) == *source.begin());
        delete inverse;
    }

    SECTION("Map image") {
        PatternGraph g1, g2;
        Node n1(typeA); g1.add_node(n1);
        Node n2(typeA); g1.add_node(n2);

        Node n3(typeB); g2.add_node(n3);
        Node n4(typeB); g2.add_node(n4);

        Map map(&g1, &g2);
        auto it1 = g1.begin();
        auto it2 = g2.begin();
        map.insert(*it1, *it2);

        ++it1; ++it2;
        map.insert(*it1, *it2);

        node_container_t img;
        map.image(g1.begin(), g1.end(), img);
        REQUIRE(img.size() == 2);

        auto it2_start = g2.begin();
        REQUIRE(img[0] == *it2_start);
        ++it2_start;
        REQUIRE(img[1] == *it2_start);
    }

    SECTION("Map quick_merge") {
        PatternGraph g1, g2;
        Node n1(typeA); g1.add_node(n1);
        Node n2(typeA); g1.add_node(n2);

        Node n3(typeB); g2.add_node(n3);
        Node n4(typeB); g2.add_node(n4);

        Map m1(&g1, &g2);
        auto it1 = g1.begin();
        auto it2 = g2.begin();
        m1.insert(*it1, *it2);

        Map m2(&g1, &g2);
        ++it1; ++it2;
        m2.insert(*it1, *it2);

        map_container_t maps;
        maps.push_back(m1);
        maps.push_back(m2);

        Map merged_map;
        merged_map.quick_merge(maps);

        REQUIRE(merged_map.size() == 2);
        REQUIRE(maps.empty());
    }

    SECTION("Map copy_map_and_target") {
        Map map(&source, &target);
        map.insert(*source.begin(), *target.begin());

        auto copy_pair = map.copy_map_and_target();
        Map* map_copy = copy_pair.first;
        PatternGraph* target_copy = copy_pair.second;

        REQUIRE(map_copy != nullptr);
        REQUIRE(target_copy != nullptr);
        REQUIRE(map_copy->get_source_graph() == &source);
        REQUIRE(map_copy->get_target_graph() == target_copy);
        REQUIRE(map_copy->size() == 1);

        // Ensure it's mapping to the new target
        REQUIRE(map_copy->mapf(*source.begin()) == *target_copy->begin());

        delete map_copy;
        delete target_copy;
    }
}
