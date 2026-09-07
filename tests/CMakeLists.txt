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

add_executable(test_psa_simulator test_psa_simulator.cpp)
target_include_directories(test_psa_simulator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser/generated
    ${antlr4_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src
    ${sundials_SOURCE_DIR}/include
    ${sundials_BINARY_DIR}/include
)
target_link_libraries(test_psa_simulator PRIVATE
    Catch2::Catch2WithMain
    Catch2::Catch2
    bng_engine
    bng_ast
    bng_core
    antlr4_static
)
catch_discover_tests(test_psa_simulator)

add_executable(test_hybrid_model_generator test_hybrid_model_generator.cpp)
target_link_libraries(test_hybrid_model_generator PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_engine bng_ast)
add_test(NAME test_hybrid_model_generator COMMAND test_hybrid_model_generator)
catch_discover_tests(test_hybrid_model_generator)

add_executable(test_network_generator test_network_generator.cpp)
target_include_directories(test_network_generator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)
target_link_libraries(test_network_generator PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_engine bng_ast bng_parser)
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


add_executable(test_cpp_export_writer test_cpp_export_writer.cpp)
target_include_directories(test_cpp_export_writer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser/generated
    ${antlr4_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src
)
target_link_libraries(test_cpp_export_writer PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_engine bng_ast bng_parser bng_core antlr4_static)
catch_discover_tests(test_cpp_export_writer)

add_executable(test_python_export_writer test_python_export_writer.cpp)

target_include_directories(test_python_export_writer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser/generated
    ${antlr4_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src
)
target_link_libraries(test_python_export_writer PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_engine bng_ast bng_core antlr4_static)
catch_discover_tests(test_python_export_writer)

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

add_test(
    NAME bng_cpp_action_errors
    COMMAND ${CMAKE_COMMAND}
        -DBNG_CPP_PATH=$<TARGET_FILE:bng_cpp>
        -DMODEL_PATH=${CMAKE_CURRENT_SOURCE_DIR}/fixtures/issue_328_missing_net.bngl
        -P ${CMAKE_CURRENT_SOURCE_DIR}/test_bng_cpp_action_errors.cmake
)

add_test(
    NAME bng_cpp_blbr_parity
    COMMAND ${CMAKE_COMMAND}
        -DBNG_CPP_PATH=$<TARGET_FILE:bng_cpp>
        -DMODEL_PATH=${CMAKE_CURRENT_SOURCE_DIR}/../bng2/Validate/blbr.bngl
        -DREFERENCE_PATH=${CMAKE_CURRENT_SOURCE_DIR}/../bng2/Validate/DAT_validate/blbr.net
        -DOUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/test_bng_cpp_blbr_parity.cmake
)

find_program(BNG_PERL_EXECUTABLE perl)
if(BNG_PERL_EXECUTABLE)
    add_test(
        NAME bng2_function_cycle
        COMMAND ${CMAKE_COMMAND}
            -DBNG_PERL_EXECUTABLE=${BNG_PERL_EXECUTABLE}
            -DBNG2_PATH=${CMAKE_CURRENT_SOURCE_DIR}/../bng2/BNG2.pl
            -DMODEL_PATH=${CMAKE_CURRENT_SOURCE_DIR}/fixtures/issue_217_circular_functions.bngl
            -P ${CMAKE_CURRENT_SOURCE_DIR}/test_bng2_function_cycle.cmake
    )

    add_test(
        NAME bng2_xml_pattern_quantifier
        COMMAND ${CMAKE_COMMAND}
            -DBNG_PERL_EXECUTABLE=${BNG_PERL_EXECUTABLE}
            -DBNG2_PATH=${CMAKE_CURRENT_SOURCE_DIR}/../bng2/BNG2.pl
            -DMODEL_PATH=${CMAKE_CURRENT_SOURCE_DIR}/fixtures/issue_234_pattern_quantifier.bngl
            -DOUTPUT_PATH=${CMAKE_CURRENT_BINARY_DIR}/issue_234_pattern_quantifier.xml
            -DEXPECTED_MARKER=relation=\"&lt;\"
            -P ${CMAKE_CURRENT_SOURCE_DIR}/test_bng2_xml_output.cmake
    )

    add_test(
        NAME bng2_xml_matchonce
        COMMAND ${CMAKE_COMMAND}
            -DBNG_PERL_EXECUTABLE=${BNG_PERL_EXECUTABLE}
            -DBNG2_PATH=${CMAKE_CURRENT_SOURCE_DIR}/../bng2/BNG2.pl
            -DMODEL_PATH=${CMAKE_CURRENT_SOURCE_DIR}/fixtures/issue_312_matchonce.bngl
            -DOUTPUT_PATH=${CMAKE_CURRENT_BINARY_DIR}/issue_312_matchonce.xml
            -DEXPECTED_MARKER=matchOnce=\"1\"
            -DEXPECTED_COUNT=2
            -P ${CMAKE_CURRENT_SOURCE_DIR}/test_bng2_xml_output.cmake
    )

    add_test(
        NAME bng2_cvode_operator_export
        COMMAND ${CMAKE_COMMAND}
            -DBNG_PERL_EXECUTABLE=${BNG_PERL_EXECUTABLE}
            -DBNG2_PATH=${CMAKE_CURRENT_SOURCE_DIR}/../bng2/BNG2.pl
            -DMODEL_PATH=${CMAKE_CURRENT_SOURCE_DIR}/fixtures/issue_295_operator_export.bngl
            -DOUTPUT_PATH=${CMAKE_CURRENT_BINARY_DIR}/issue_295_operator_export_cvode.c
            -DEXPECTED_MARKER=1.0!=0.0
            -P ${CMAKE_CURRENT_SOURCE_DIR}/test_bng2_text_output.cmake
    )

    add_test(
        NAME bng2_console_errors_on_stderr
        COMMAND ${CMAKE_COMMAND}
            -DBNG_PERL_EXECUTABLE=${BNG_PERL_EXECUTABLE}
            -DBNG2_PATH=${CMAKE_CURRENT_SOURCE_DIR}/../bng2/BNG2.pl
            -DINPUT_PATH=${CMAKE_CURRENT_SOURCE_DIR}/fixtures/issue_112_console_action.in
            -P ${CMAKE_CURRENT_SOURCE_DIR}/test_bng2_console_streams.cmake
    )

    add_test(
        NAME bng2_implicit_bond_symmetry_factor
        COMMAND ${CMAKE_COMMAND}
            -DBNG_PERL_EXECUTABLE=${BNG_PERL_EXECUTABLE}
            -DBNG2_PATH=${CMAKE_CURRENT_SOURCE_DIR}/../bng2/BNG2.pl
            -DMODEL_PATH=${CMAKE_CURRENT_SOURCE_DIR}/fixtures/issue_090_implicit_bonds.bngl
            -DOUTPUT_PATH=${CMAKE_CURRENT_BINARY_DIR}/issue_090_implicit_bonds.net
            -P ${CMAKE_CURRENT_SOURCE_DIR}/test_bng2_symmetry_factor.cmake
    )
endif()

add_executable(test_GraphTypeRegistry ast/test_GraphTypeRegistry.cpp)

add_executable(test_Compartment ast/test_Compartment.cpp)
target_include_directories(test_Compartment PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ast
)
target_link_libraries(test_Compartment PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_ast)
catch_discover_tests(test_Compartment)
target_include_directories(test_GraphTypeRegistry PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ast
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core
)
target_link_libraries(test_GraphTypeRegistry PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_ast bng_core)
add_executable(test_Species ast/test_Species.cpp)
add_executable(test_SpeciesList ast/test_SpeciesList.cpp)

target_include_directories(test_SpeciesList PRIVATE

    ${CMAKE_CURRENT_SOURCE_DIR}/../src

)

target_link_libraries(test_SpeciesList PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_ast bng_core)

catch_discover_tests(test_SpeciesList)
target_include_directories(test_Species PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)
target_link_libraries(test_Species PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_ast bng_core)
catch_discover_tests(test_Species)

catch_discover_tests(test_GraphTypeRegistry)

add_executable(test_Observable ast/test_Observable.cpp)
target_include_directories(test_Observable PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ast
)
target_link_libraries(test_Observable PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_ast)
catch_discover_tests(test_Observable)

add_executable(test_ode_integrator test_ode_integrator.cpp)
target_include_directories(test_ode_integrator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser/generated
    ${antlr4_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src
    ${sundials_SOURCE_DIR}/include
    ${sundials_BINARY_DIR}/include
)
target_link_libraries(test_ode_integrator PRIVATE
    Catch2::Catch2WithMain
    Catch2::Catch2
    bng_engine
    bng_ast
    bng_core
    antlr4_static
)
catch_discover_tests(test_ode_integrator)
add_executable(test_pattern_graph test_pattern_graph.cpp)
target_include_directories(test_pattern_graph PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src ${CMAKE_CURRENT_SOURCE_DIR}/../src/core)
target_link_libraries(test_pattern_graph PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_core)
catch_discover_tests(test_pattern_graph)

add_executable(test_map test_map.cpp)
target_include_directories(test_map PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core
)
target_link_libraries(test_map PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_core)
catch_discover_tests(test_map)

add_executable(test_matlab_writer test_matlab_writer.cpp)
target_include_directories(test_matlab_writer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser/generated
    ${antlr4_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src
)
target_link_libraries(test_matlab_writer PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_engine bng_ast bng_core antlr4_static)
catch_discover_tests(test_matlab_writer)

add_executable(test_console test_console.cpp)
target_include_directories(test_console PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/console
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ast
)
target_link_libraries(test_console PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_engine bng_ast bng_core antlr4_static)
catch_discover_tests(test_console)

add_executable(test_latex_writer test_latex_writer.cpp)
target_include_directories(test_latex_writer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser/generated
    ${antlr4_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src
)
target_link_libraries(test_latex_writer PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_engine bng_ast bng_core antlr4_static)
catch_discover_tests(test_latex_writer)

add_executable(test_sbml_writer test_sbml_writer.cpp)
target_include_directories(test_sbml_writer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser/generated
    ${antlr4_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src
)
target_link_libraries(test_sbml_writer PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_engine bng_ast bng_core antlr4_static)
catch_discover_tests(test_sbml_writer)

add_executable(test_MoleculeType ast/test_MoleculeType.cpp)
target_include_directories(test_MoleculeType PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ast
)
target_link_libraries(test_MoleculeType PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_ast)
catch_discover_tests(test_MoleculeType)

add_executable(benchmark benchmark.cpp)
target_include_directories(benchmark PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser/generated
    ${antlr4_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src
    ${sundials_SOURCE_DIR}/include
    ${sundials_BINARY_DIR}/include
)
target_link_libraries(benchmark PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_engine bng_ast bng_parser bng_core antlr4_static)
catch_discover_tests(benchmark)

add_executable(test_bngl_writer test_bngl_writer.cpp)
target_include_directories(test_bngl_writer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser/generated
    ${antlr4_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src
)
target_link_libraries(test_bngl_writer PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_engine bng_ast bng_core antlr4_static)
catch_discover_tests(test_bngl_writer)

add_executable(test_ssc_writer test_ssc_writer.cpp)
target_include_directories(test_ssc_writer PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}/../antlr4_generated/src
)
target_link_libraries(test_ssc_writer PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_engine bng_ast bng_core antlr4_static)
catch_discover_tests(test_ssc_writer)

add_executable(test_michaelis_menten
    test_michaelis_menten.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../bng2/Network3/src/util/misc.cpp
)
target_include_directories(test_michaelis_menten PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../bng2/Network3/src
)
target_link_libraries(test_michaelis_menten PRIVATE Catch2::Catch2WithMain Catch2::Catch2 bng_ast)
catch_discover_tests(test_michaelis_menten)
