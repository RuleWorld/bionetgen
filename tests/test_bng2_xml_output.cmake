execute_process(
    COMMAND "${BNG_PERL_EXECUTABLE}" "${BNG2_PATH}" "${MODEL_PATH}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error
)

if(NOT result EQUAL 0)
    message(FATAL_ERROR "BNG2 XML fixture failed with ${result}. Output:\n${output}${error}")
endif()

if(NOT EXISTS "${OUTPUT_PATH}")
    message(FATAL_ERROR "Expected XML output was not created: ${OUTPUT_PATH}")
endif()

file(READ "${OUTPUT_PATH}" xml)
string(FIND "${xml}" "${EXPECTED_MARKER}" marker_index)
if(marker_index EQUAL -1)
    message(FATAL_ERROR "Expected XML marker missing: ${EXPECTED_MARKER}")
endif()

if(DEFINED EXPECTED_COUNT)
    string(REGEX MATCHALL "${EXPECTED_MARKER}" matches "${xml}")
    list(LENGTH matches marker_count)
    if(NOT marker_count EQUAL EXPECTED_COUNT)
        message(FATAL_ERROR "Expected ${EXPECTED_COUNT} XML markers, found ${marker_count}: ${EXPECTED_MARKER}")
    endif()
endif()
