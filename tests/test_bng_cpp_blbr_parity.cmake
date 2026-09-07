file(COPY "${MODEL_PATH}" DESTINATION "${OUTPUT_DIR}")
get_filename_component(model_name "${MODEL_PATH}" NAME)
set(local_model "${OUTPUT_DIR}/${model_name}")

execute_process(
    COMMAND "${BNG_CPP_PATH}" "${local_model}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error
)

if(NOT result EQUAL 0)
    message(FATAL_ERROR "blbr generation failed with ${result}. Output:\n${output}${error}")
endif()

get_filename_component(model_stem "${model_name}" NAME_WE)
set(generated_net "${OUTPUT_DIR}/${model_stem}.net")
if(NOT EXISTS "${generated_net}")
    message(FATAL_ERROR "Generated network was not created: ${generated_net}")
endif()

function(count_reactions net_path output_var)
    file(STRINGS "${net_path}" lines)
    set(in_reactions FALSE)
    set(count 0)
    foreach(line IN LISTS lines)
        if(line STREQUAL "begin reactions")
            set(in_reactions TRUE)
        elseif(line STREQUAL "end reactions")
            set(in_reactions FALSE)
        elseif(in_reactions AND line MATCHES "^[ \t]*[0-9]+[ \t]")
            math(EXPR count "${count} + 1")
        endif()
    endforeach()
    set(${output_var} "${count}" PARENT_SCOPE)
endfunction()

count_reactions("${generated_net}" generated_count)
count_reactions("${REFERENCE_PATH}" reference_count)

if(NOT generated_count EQUAL reference_count)
    message(FATAL_ERROR
        "Native blbr reaction count differs from the checked-in reference: "
        "generated=${generated_count}, reference=${reference_count}")
endif()
