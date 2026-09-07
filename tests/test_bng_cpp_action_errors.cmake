execute_process(
    COMMAND "${BNG_CPP_PATH}" "${MODEL_PATH}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE stdout_text
    ERROR_VARIABLE stderr_text
)

if(NOT result EQUAL 1)
    message(FATAL_ERROR
        "bng_cpp should report action errors with exit code 1, got '${result}'.\n"
        "stdout:\n${stdout_text}\nstderr:\n${stderr_text}")
endif()

if(NOT stderr_text MATCHES "action execution failed")
    message(FATAL_ERROR
        "bng_cpp did not report the action failure on stderr:\n${stderr_text}")
endif()
