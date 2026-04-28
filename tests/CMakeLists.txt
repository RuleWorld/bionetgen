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
