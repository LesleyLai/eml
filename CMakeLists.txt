cmake_minimum_required(VERSION 3.8)

project(EmbeddedML
        VERSION 0.1.0
        LANGUAGES CXX
        )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN TRUE)

include("CMakeDependentOption")

option(EML_WARNING_AS_ERROR "Treats compiler warnings as errors" ON)
option(EML_ENABLE_CLANG_TIDY "Enable clang tidy checks" OFF)


option(EML_DEBUG_PRINT_AST "The Parser will print the generated ast" OFF)
option(EML_DEBUG_VM_TRACE_EXECUTION
        "The VM will disassemble all the instruction when running with this option"
        OFF)

option(EML_BUILD_DOCUMENTS "Builds the documents for EML" OFF)
option(EML_BUILD_TESTS "Builds the tests for EML" OFF)
CMAKE_DEPENDENT_OPTION(EML_BUILD_TESTS_COVERAGE
        "Build the project with code coverage support for tests,
    must compile with a gcc-compatible compiler" OFF
        "EML_BUILD_TESTS" OFF)

if (EML_ENABLE_CLANG_TIDY)
    include("cmake/clang-tidy.cmake")
endif ()
include("compiler")
#include("cmake/cppcheck.cmake")
if (EML_BUILD_DOCUMENTS)
    include("doxygen")
endif ()

configure_file("src/eml_version.cpp.in" eml_version.cpp @ONLY)
add_library(eml_version STATIC ${CMAKE_CURRENT_BINARY_DIR}/eml_version.cpp)
target_link_libraries(eml_version PRIVATE compiler_options)

add_library(eml
        "src/ast.hpp"
        "src/bytecode.hpp"
        "src/common.hpp"
        "src/compiler.hpp"
        "src/code_generator.cpp"
        "src/debug.hpp"
        "src/debug.cpp"
        "src/eml.hpp"
        "src/expected.hpp"
        "src/error.hpp"
        "src/error.cpp"
        "src/memory.hpp"
        "src/memory.cpp"
        "src/module.hpp"
        "src/module.cpp"
        "src/parser.hpp"
        "src/parser.cpp"
        "src/string.hpp"
        "src/string.cpp"
        "src/token_table.inc"
        "src/type.hpp"
        "src/type.cpp"
        "src/type_checker.cpp"
        "src/scanner.hpp"
        "src/scanner.cpp"
        "src/value.hpp"
        "src/value.cpp"
        "src/vm.hpp"
        "src/vm.cpp")
target_link_libraries(eml PRIVATE compiler_options eml_version)
target_include_directories(eml PUBLIC "src")

add_library(eml::eml ALIAS eml)

# Disassembler
add_subdirectory("src/disassembler")

if (${EML_BUILD_TESTS_COVERAGE})
    target_compile_options(eml PRIVATE -fprofile-arcs -ftest-coverage)
    target_link_libraries(eml PRIVATE gcov)
endif ()

# The standalone interpreter of the embedded ML
add_subdirectory("src/cli")

target_compile_definitions(eml PRIVATE $<$<CONFIG:Debug>:EML_DEBUG>)

if (EML_DEBUG_PRINT_AST)
    target_compile_definitions(eml PRIVATE EML_DEBUG_PRINT_AST)
endif ()

if (EML_BUILD_TESTS)
    # Conan package manager
    if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
        file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake")
    endif ()

    include(${CMAKE_BINARY_DIR}/conan.cmake)

    add_subdirectory(tests)
    enable_testing()
endif ()


