cmake_minimum_required(VERSION 3.8)

project(EmbeddedML
    VERSION 0.0.0
    )

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include("cmake/compiler.cmake")
#include("cmake/clang-tidy.cmake")
#include("cmake/cppcheck.cmake")
include("cmake/doxygen.cmake")

option(EML_VM_DEBUG_TRACE_EXECUTION
    "The CM will disassemble all the instruction when running with this option"
    OFF)

add_library(lib${PROJECT_NAME}
    "src/ast.hpp"
    "src/common.hpp"
    "src/compiler.hpp"
    "src/compiler.cpp"
    "src/debug.hpp"
    "src/debug.cpp"
    "src/expected.hpp"
    "src/error.hpp"
    "src/error.cpp"
    "src/parser.hpp"
    "src/parser.cpp"
    "src/token_table.inc"
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

if(EML_VM_DEBUG_TRACE_EXECUTION)
    target_compile_definitions(lib${PROJECT_NAME} PRIVATE EML_VM_DEBUG_TRACE_EXECUTION)
endif()

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

