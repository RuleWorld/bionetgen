# Src CMakeLists.txt

# --- Nauty Library ---
# Simply compile nauty sources into a static library.
file(GLOB NAUTY_SOURCES "nauty/*.c")
add_library(nauty STATIC ${NAUTY_SOURCES})
target_include_directories(nauty PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/nauty)

# --- BNG Core Library ---
file(GLOB BNG_CORE_SOURCES "core/*.cpp")
# There are SWIG wrappers in there we need to exclude
list(FILTER BNG_CORE_SOURCES EXCLUDE REGEX ".*_wrap\\.cxx")
add_library(bng_core STATIC ${BNG_CORE_SOURCES})
target_include_directories(bng_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/core)
target_link_libraries(bng_core PUBLIC nauty)

# --- AST Library ---
add_library(bng_ast
    ast/Model.cpp
    ast/Compartment.cpp
    ast/EnergyPattern.cpp
    ast/Molecule.cpp
    ast/Expression.cpp
    ast/Function.cpp
    ast/GraphTypeRegistry.cpp
    ast/MoleculeType.cpp
    ast/Observable.cpp
    ast/Parameter.cpp
    ast/ParameterList.cpp
    ast/ReactionRule.cpp
    ast/SeedSpecies.cpp
    ast/SpeciesGraph.cpp
    ast/Species.cpp
    ast/SpeciesList.cpp
    ast/Rxn.cpp
    ast/RxnList.cpp
)
target_include_directories(bng_ast PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/generated
    ${CMAKE_CURRENT_SOURCE_DIR}/parser
    ${antlr4_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src
)
target_link_libraries(bng_ast PUBLIC bng_core antlr4_static)

# --- ANTLR4 Parser Generation ---
# The generated files are assumed to be in ${CMAKE_CURRENT_SOURCE_DIR}/parser/generated
set(ANTLR_GEN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/parser/generated)
add_library(bng_parser STATIC
    ${ANTLR_GEN_DIR}/BNGLexer.cpp
    ${ANTLR_GEN_DIR}/BNGParser.cpp
    ${ANTLR_GEN_DIR}/BNGParserBaseVisitor.cpp
    ${ANTLR_GEN_DIR}/BNGParserVisitor.cpp
    parser/BNGAstVisitor.cpp
    parser/PatternGraphBuilder.cpp
)
target_include_directories(bng_parser PUBLIC 
    ${ANTLR_GEN_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/parser
    ${antlr4_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src
)
# Linking against antlr4_shared (from FetchContent in root)
target_link_libraries(bng_parser PUBLIC antlr4_static bng_ast)

add_library(bng_engine STATIC
    engine/NetworkGenerator.cpp
    engine/OdeIntegrator.cpp
    io/NetWriter.cpp
    io/NetReader.cpp
    io/XmlWriter.cpp
    io/BnglWriter.cpp
    io/SbmlWriter.cpp
    io/MatlabWriter.cpp
    io/ContactMapWriter.cpp
    actions/ActionDispatch.cpp
)

# Link SUNDIALS/CVODE from FetchContent
target_include_directories(bng_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ANTLR_GEN_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/parser
    ${antlr4_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src
)
target_link_libraries(bng_engine PUBLIC
    bng_parser
    bng_ast
    bng_core
    antlr4_static
    sundials_cvode_static
    sundials_nvecserial_static
)
target_compile_definitions(bng_engine PUBLIC BNG_CPP_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

# --- Main Executable ---
add_executable(bng_cpp main.cpp)
target_link_libraries(bng_cpp PRIVATE bng_engine bng_parser bng_ast bng_core nauty)
target_compile_definitions(bng_cpp PRIVATE BNG_CPP_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
