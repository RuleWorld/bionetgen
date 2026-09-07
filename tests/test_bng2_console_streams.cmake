execute_process(
    COMMAND "${BNG_PERL_EXECUTABLE}" "${BNG2_PATH}" --console
    INPUT_FILE "${INPUT_PATH}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error
)

if(NOT result EQUAL 0)
    message(FATAL_ERROR "BNG2 console fixture failed with ${result}. Output:\n${output}${error}")
endif()

if(output MATCHES "ERROR:")
    message(FATAL_ERROR "Console error was written to stdout:\n${output}")
endif()

if(NOT error MATCHES "ERROR: Attempt to execute action without loading model")
    message(FATAL_ERROR "Console error was not written to stderr:\n${error}")
endif()
