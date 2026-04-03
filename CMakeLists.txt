cmake_minimum_required(VERSION 3.14)
project(BioNetGen_CPP LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
endif()

# Bring in Catch2 (Tests)
include(FetchContent)
if(EXISTS "${CMAKE_SOURCE_DIR}/build/_deps/catch2-src/CMakeLists.txt")
  set(FETCHCONTENT_SOURCE_DIR_CATCH2 "${CMAKE_SOURCE_DIR}/build/_deps/catch2-src")
endif()

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(Catch2)

# Local nauty and BNGcore are built in the src directory

# Fetch ANTLR4 C++ Runtime
if(EXISTS "${CMAKE_SOURCE_DIR}/build/_deps/antlr4_runtime-src/runtime/Cpp/CMakeLists.txt")
    set(FETCHCONTENT_SOURCE_DIR_ANTLR4_RUNTIME "${CMAKE_SOURCE_DIR}/build/_deps/antlr4_runtime-src")
endif()

FetchContent_Declare(
    antlr4_runtime
    GIT_REPOSITORY https://github.com/antlr/antlr4.git
    GIT_TAG        4.13.1
    SOURCE_SUBDIR  runtime/Cpp
)

set(ANTLR4_INSTALL ON CACHE INTERNAL "")
set(ANTLR4_WITH_LIBCXX OFF CACHE INTERNAL "")
set(ANTLR_BUILD_CPP_TESTS OFF CACHE INTERNAL "")
# Generate static library
set(ANTLR4_STATIC ON CACHE INTERNAL "")
set(ANTLR4_SHARED OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(antlr4_runtime)

# Fetch SUNDIALS (CVODE solver)
if(EXISTS "${CMAKE_SOURCE_DIR}/build/_deps/sundials-src/CMakeLists.txt")
    set(FETCHCONTENT_SOURCE_DIR_SUNDIALS "${CMAKE_SOURCE_DIR}/build/_deps/sundials-src")
endif()

FetchContent_Declare(
  sundials
  GIT_REPOSITORY https://github.com/LLNL/sundials.git
  GIT_TAG        v2.7.0   # Compatible with CVODE 2.6.0 API
)

# Configure SUNDIALS build options - only build CVODE, no tests/examples
set(BUILD_CVODE ON CACHE INTERNAL "Build CVODE solver")
set(BUILD_CVODES OFF CACHE INTERNAL "")
set(BUILD_IDA OFF CACHE INTERNAL "")
set(BUILD_IDAS OFF CACHE INTERNAL "")
set(BUILD_KINSOL OFF CACHE INTERNAL "")
set(BUILD_ARKODE OFF CACHE INTERNAL "")
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Build static libraries")
set(BUILD_STATIC_LIBS ON CACHE INTERNAL "Build static libraries")
set(BUILD_TESTING OFF CACHE INTERNAL "Disable SUNDIALS tests")
set(EXAMPLES_ENABLE OFF CACHE INTERNAL "Disable all SUNDIALS examples")
set(EXAMPLES_ENABLE_C OFF CACHE INTERNAL "Disable C examples")
set(EXAMPLES_INSTALL OFF CACHE INTERNAL "Don't install examples")

FetchContent_MakeAvailable(sundials)

# Enable testing AFTER FetchContent blocks so third-party tests
# (e.g. SUNDIALS's Python-2-era test runner) don't get registered
enable_testing()
include(CTest)

# Source directory
add_subdirectory(src)

# Tests directory
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt)
    add_subdirectory(tests)
endif()
