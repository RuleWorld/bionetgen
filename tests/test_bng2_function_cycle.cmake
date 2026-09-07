execute_process(
    COMMAND "${BNG_PERL_EXECUTABLE}" "${BNG2_PATH}" "${MODEL_PATH}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error
)

if(result EQUAL 0)
    message(FATAL_ERROR "Circular function model unexpectedly succeeded")
endif()

set(combined "${output}${error}")
string(FIND "${combined}" "Function dependency cycle" cycle_index)
if(cycle_index EQUAL -1)
    message(FATAL_ERROR "Cycle diagnostic missing. Output:\n${combined}")
endif()

string(FIND "${combined}" "Deep recursion" recursion_index)
if(NOT recursion_index EQUAL -1)
    message(FATAL_ERROR "Cycle still reached recursive overflow. Output:\n${combined}")
endif()
