cmake_minimum_required(VERSION 3.8)

project(EmbeddedML
    VERSION 0.0.0
    LANGUAGES CXX
    )

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include("CMakeDependentOption")

include("cmake/compiler.cmake")
#include("cmake/cppcheck.cmake")

option(EML_ENABLE_CLANG_TIDY
    "Enable clang tidy checks"
    OFF)

if(EML_ENABLE_CLANG_TIDY)
    include("cmake/clang-tidy.cmake")
endif()

option(EML_DEBUG_PRINT_AST
    "The Parser will print the generated ast"
    OFF)

option(EML_DEBUG_VM_TRACE_EXECUTION
    "The VM will disassemble all the instruction when running with this option"
    OFF)

option(EML_BUILD_DOCUMENTS
    "Builds the documents for EML"
    OFF)

option(EML_BUILD_TESTS
    "Builds the tests for EML"
    OFF)

CMAKE_DEPENDENT_OPTION(EML_BUILD_TESTS_COVERAGE
    "Build the project with code coverage support for tests,
    must compile with a gcc-compatible compiler" OFF
                       "EML_BUILD_TESTS" OFF)

if(EML_BUILD_DOCUMENTS)
    include("cmake/doxygen.cmake")
endif()

add_library(eml
    "src/ast.hpp"
    "src/bytecode.hpp"
    "src/bytecode.cpp"
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
    "src/meta.hpp"
    "src/module.hpp"
    "src/module.cpp"
    "src/object.hpp"
    "src/parser.hpp"
    "src/parser.cpp"
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
target_include_directories(eml PUBLIC "src")

if(${EML_BUILD_TESTS_COVERAGE})
    target_compile_options(eml PRIVATE -fprofile-arcs -ftest-coverage)
    target_link_libraries(eml gcov)
endif()

# The standalone interpreter of the embedded ML
add_subdirectory("src/cli")

target_compile_definitions(eml PUBLIC
    EML_VERSION="${PROJECT_VERSION}"
    EML_MAJOR_VERSION=${PROJECT_VERSION_MAJOR}
    EML_MINOR_VERSION=${PROJECT_VERSION_MINOR}
    EML_PATCH_VERSION=${PROJECT_VERSION_PATCH})

target_compile_definitions(eml PRIVATE $<$<CONFIG:Debug>:EML_DEBUG>)

if(EML_DEBUG_VM_TRACE_EXECUTION)
    target_compile_definitions(eml PRIVATE EML_DEBUG_VM_TRACE_EXECUTION)
endif()

if(EML_DEBUG_PRINT_AST)
    target_compile_definitions(eml PRIVATE EML_DEBUG_PRINT_AST)
endif()

if(EML_BUILD_TESTS)
    # Conan package manager
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
       message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
       file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                     "${CMAKE_BINARY_DIR}/conan.cmake")
    endif()

    include(${CMAKE_BINARY_DIR}/conan.cmake)

    add_subdirectory(tests)
    enable_testing()
endif()


