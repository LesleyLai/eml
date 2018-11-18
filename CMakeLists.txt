cmake_minimum_required(VERSION 3.8)

project(EmbeddedML)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include("cmake/compiler.cmake")
#include("cmake/clang-tidy.cmake")
#include("cmake/cppcheck.cmake")

option(EML_VM_DEBUG_TRACE_EXECUTION
    "The CM will disassemble all the instruction when running with this option"
    OFF)

if(EML_VM_DEBUG_TRACE_EXECUTION)
add_definitions(-DEML_VM_DEBUG_TRACE_EXECUTION)
endif()

add_library(lib${PROJECT_NAME} "token_table.impl" "scanner.hpp" "scanner.cpp" "value.hpp" "vm.hpp" "vm.cpp")

add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} lib${PROJECT_NAME})

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

