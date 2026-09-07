execute_process(
    COMMAND "${BNG_PERL_EXECUTABLE}" "${BNG2_PATH}" "${MODEL_PATH}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error
)

if(NOT result EQUAL 0)
    message(FATAL_ERROR "BNG2 export fixture failed with ${result}. Output:\n${output}${error}")
endif()

if(NOT EXISTS "${OUTPUT_PATH}")
    message(FATAL_ERROR "Expected export output was not created: ${OUTPUT_PATH}")
endif()

file(READ "${OUTPUT_PATH}" generated)
string(FIND "${generated}" "${EXPECTED_MARKER}" marker_index)
if(marker_index EQUAL -1)
    message(FATAL_ERROR "Expected export marker missing: ${EXPECTED_MARKER}")
endif()

string(FIND "${generated}" "(!NV_Ith_S" second_marker_index)
if(second_marker_index EQUAL -1)
    message(FATAL_ERROR "Expected unary-not conversion missing from generated export")
endif()
