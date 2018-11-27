cmake_minimum_required(VERSION 3.8)

project(EmbeddedML
    VERSION 0.0.0
    )

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include("CMakeDependentOption")

include("cmake/compiler.cmake")
#include("cmake/clang-tidy.cmake")
#include("cmake/cppcheck.cmake")


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
    "Build the project with code coverage support for tests" OFF
                       "EML_BUILD_TESTS" OFF)

if(EML_BUILD_DOCUMENTS)
    include("cmake/doxygen.cmake")
endif()

add_library(lib${PROJECT_NAME}
    "src/ast.hpp"
    "src/common.hpp"
    "src/compiler.hpp"
    "src/compiler.cpp"
    "src/debug.hpp"
    "src/debug.cpp"
    "src/eml.hpp"
    "src/eml.cpp"
    "src/expected.hpp"
    "src/error.hpp"
    "src/error.cpp"
    "src/parser.hpp"
    "src/parser.cpp"
    "src/opcode.inc"
    "src/token_table.inc"
    "src/type.hpp"
    "src/type.cpp"
    "src/type_checker.hpp"
    "src/type_checker.cpp"
    "src/scanner.hpp"
    "src/scanner.cpp"
    "src/value.hpp"
    "src/value.cpp"
    "src/vm.hpp"
    "src/vm.cpp")
target_include_directories(lib${PROJECT_NAME} PUBLIC "src")

add_executable(${PROJECT_NAME} "src/main.cpp")
target_link_libraries(${PROJECT_NAME} lib${PROJECT_NAME})

target_compile_definitions(lib${PROJECT_NAME} PUBLIC
    EML_VERSION="${PROJECT_VERSION}"
    EML_MAJOR_VERSION=${PROJECT_VERSION_MAJOR}
    EML_MINOR_VERSION=${PROJECT_VERSION_MINOR}
    EML_PATCH_VERSION=${PROJECT_VERSION_PATCH})

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_definitions(lib${PROJECT_NAME} PRIVATE EML_DEBUG)
endif()

if(EML_DEBUG_VM_TRACE_EXECUTION)
    target_compile_definitions(lib${PROJECT_NAME} PRIVATE EML_DEBUG_VM_TRACE_EXECUTION)
endif()

if(EML_DEBUG_PRINT_AST)
    target_compile_definitions(lib${PROJECT_NAME} PRIVATE EML_DEBUG_PRINT_AST)
endif()

if(EML_BUILD_TESTS)
    # Conan package manager
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
       message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
       file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                     "${CMAKE_BINARY_DIR}/conan.cmake")
    endif()

    include(${CMAKE_BINARY_DIR}/conan.cmake)
    conan_cmake_run(REQUIRES catch2/2.4.0@bincrafters/stable
                    BASIC_SETUP CMAKE_TARGETS
                    BUILD missing)

    add_subdirectory(tests)
    enable_testing()
endif()


