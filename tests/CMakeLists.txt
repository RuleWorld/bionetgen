add_executable(bng_tests
    test_parser.cpp
    test_expression.cpp
    test_species_graph.cpp
    test_visitor.cpp
    test_semantic_models.cpp
    test_network_generation.cpp
)

target_link_libraries(bng_tests PRIVATE
    Catch2::Catch2WithMain
    bng_engine
    bng_parser
    bng_ast
    bng_core
    nauty
    antlr4_static
)

target_include_directories(bng_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/parser/generated
    ${antlr4_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src
)
target_compile_definitions(bng_tests PRIVATE BNG_CPP_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

include(Catch)
catch_discover_tests(bng_tests)
