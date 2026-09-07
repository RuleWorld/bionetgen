get_filename_component(model_name "${MODEL_PATH}" NAME_WE)
get_filename_component(output_dir "${OUTPUT_PATH}" DIRECTORY)

execute_process(
    COMMAND "${BNG_PERL_EXECUTABLE}" "${BNG2_PATH}" --outdir "${output_dir}" "${MODEL_PATH}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error
)

if(NOT result EQUAL 0)
    message(FATAL_ERROR "BNG2 symmetry fixture failed with ${result}. Output:\n${output}${error}")
endif()

set(network_path "${output_dir}/${model_name}.net")
if(NOT EXISTS "${network_path}")
    message(FATAL_ERROR "Expected network output was not created: ${network_path}")
endif()

file(READ "${network_path}" network)
string(FIND "${network}" "0.5*_rateLaw1" expected_index)
if(expected_index EQUAL -1)
    message(FATAL_ERROR "Expected symmetry factor 0.5 missing from network:\n${network}")
endif()

string(FIND "${network}" "0.25*_rateLaw1" old_index)
if(NOT old_index EQUAL -1)
    message(FATAL_ERROR "Incorrect symmetry factor 0.25 remains in network:\n${network}")
endif()
