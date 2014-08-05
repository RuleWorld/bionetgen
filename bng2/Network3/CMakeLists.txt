cmake_minimum_required(VERSION 2.6)

project(Network3)

add_definitions(-DRUN_NETWORK_VERSION=\"3.0\")
#add_definitions("-stdlib=libstdc++")

set(SUB_DIRS
src
src/model
src/pla
src/ssa
src/util
src/model/rateExpressions
src/model/reactions
src/pla/base
src/pla/eRungeKutta
src/pla/fEuler
src/pla/util
src/pla/eRungeKutta/base
src/pla/eRungeKutta/util
src/util/MTrand
src/util/rand2
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} include include/cvode include/nvector include/sundials SUB_DIRS)

aux_source_directory(src src_files)
aux_source_directory(src/model model_files)
aux_source_directory(src/pla pla_files)
aux_source_directory(src/ssa ssa_files)
aux_source_directory(src/util src_util_files)
aux_source_directory(src/model/rateExpressions rexp_files)
aux_source_directory(src/model/reactions rxn_files)
aux_source_directory(src/pla/base pla_base_files)
aux_source_directory(src/pla/eRungeKutta eRK_files)
aux_source_directory(src/pla/fEuler fEuler_files)
aux_source_directory(src/pla/util pla_util_files)
aux_source_directory(src/pla/eRungeKutta/base eRK_base_files)
aux_source_directory(src/pla/eRungeKutta/util eRK_util_files)
aux_source_directory(src/util/MTrand mtrand_files)
aux_source_directory(src/util/rand2 rand2_files)

set(SRC_FILES
${src_files}
${model_files}
${pla_files}
${ssa_files}
${src_util_files}
${rexp_files}
${rxn_files}
${pla_base_files}
${eRK_files}
${fEuler_files}
${pla_util_files}
${eRK_base_files}
${eRK_util_files}
${mtrand_files}
${rand2_files}
)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

link_libraries(libmathutils.a libmuparser.a libsundials_cvode.a libsundials_nvecserial.a)

#set(CMAKE_EXE_LINKER_FLAGS "-stdlib=libstdc++")

add_executable(run_network ${SRC_FILES})
