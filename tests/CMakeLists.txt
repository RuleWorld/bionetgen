add_executable(localfunc_test localfunc_test.cpp)
target_include_directories(localfunc_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks
    ${sundials_SOURCE_DIR}/include
    ${sundials_BINARY_DIR}/include
)
target_link_libraries(localfunc_test PRIVATE
    Catch2::Catch2WithMain
    sundials_nvecserial_static
    sundials_cvode_static
)
add_test(NAME localfunc_test COMMAND localfunc_test)

add_executable(test_run_network test_run_network.cpp)
target_link_libraries(test_run_network PRIVATE Catch2::Catch2WithMain Catch2::Catch2)

# Pass the absolute path to the compiled run_network executable
target_compile_definitions(test_run_network PRIVATE
    RUN_NETWORK_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../bng2/Network3/run_network"
)

include(Catch)
catch_discover_tests(test_run_network)

add_executable(test_pla_simulator test_pla_simulator.cpp)
target_include_directories(test_pla_simulator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser/generated
    ${antlr4_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src
    ${sundials_SOURCE_DIR}/include
    ${sundials_BINARY_DIR}/include
)
target_link_libraries(test_pla_simulator PRIVATE
    Catch2::Catch2WithMain
    Catch2::Catch2
    bng_engine
    bng_ast
    bng_core
    antlr4_static
)
catch_discover_tests(test_pla_simulator)

add_executable(test_hybrid_model_generator test_hybrid_model_generator.cpp)
target_link_libraries(test_hybrid_model_generator PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_engine bng_ast)
add_test(NAME test_hybrid_model_generator COMMAND test_hybrid_model_generator)
catch_discover_tests(test_hybrid_model_generator)

add_executable(test_network_generator test_network_generator.cpp)
target_include_directories(test_network_generator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)
target_link_libraries(test_network_generator PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_engine bng_ast)
add_test(NAME test_network_generator COMMAND test_network_generator)
catch_discover_tests(test_network_generator)

add_executable(test_node_operator test_node_operator.cpp)
target_include_directories(test_node_operator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core
)
target_link_libraries(test_node_operator PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_core)
catch_discover_tests(test_node_operator)

add_executable(test_model test_model.cpp)
target_include_directories(test_model PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ast
)
target_link_libraries(test_model PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_ast bng_parser bng_core antlr4_static)
catch_discover_tests(test_model)

add_executable(test_net_writer test_net_writer.cpp)
target_include_directories(test_net_writer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser/generated
    ${antlr4_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src
)
target_link_libraries(test_net_writer PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_engine bng_ast bng_core antlr4_static)
catch_discover_tests(test_net_writer)

add_executable(test_evaluate_expression_string test_evaluate_expression_string.cpp)
target_include_directories(test_evaluate_expression_string PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser/generated
    ${antlr4_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src
)
target_link_libraries(test_evaluate_expression_string PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_engine bng_ast bng_core antlr4_static)
catch_discover_tests(test_evaluate_expression_string)

add_executable(test_regulatory_graph_writer test_regulatory_graph_writer.cpp)
target_include_directories(test_regulatory_graph_writer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser/generated
    ${antlr4_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src
)
target_link_libraries(test_regulatory_graph_writer PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_engine bng_ast bng_core antlr4_static)
catch_discover_tests(test_regulatory_graph_writer)

add_executable(test_action_dispatch test_action_dispatch.cpp)
target_include_directories(test_action_dispatch PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser/generated
    ${antlr4_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src
    ${sundials_SOURCE_DIR}/include
    ${sundials_BINARY_DIR}/include
)
target_link_libraries(test_action_dispatch PRIVATE
    Catch2::Catch2WithMain
    Catch2::Catch2
    bng_engine
    bng_ast
    bng_core
    antlr4_static
)
catch_discover_tests(test_action_dispatch)
