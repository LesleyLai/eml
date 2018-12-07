include(CTest)
set(EML_TEST_TARGET ${PROJECT_NAME}Test)
add_executable(${EML_TEST_TARGET}
    "expected/assignment.cpp"
    "expected/bases.cpp"
    "expected/constructors.cpp"
    "expected/emplace.cpp"
    "expected/extensions.cpp"
    "expected/issues.cpp"
    "expected/observers.cpp"
    "main.cpp"
    "ast_test.cpp"
    "parser_test.cpp"
    "scanner_test.cpp"
    "value_test.cpp"
    "vm_test_util.hpp"
    "vm_test.cpp"
    "type_check_test.cpp"
    )

target_link_libraries(${EML_TEST_TARGET} lib${PROJECT_NAME} CONAN_PKG::catch2)

if(${EML_BUILD_TESTS_COVERAGE})
    include(ProcessorCount)
    ProcessorCount(PROCESSOR_COUNT)

    set(CMAKE_STATIC_LINKER_FLAGS "--whole-archive")
    set(CMAKE_SHARED_LINKER_FLAGS "--whole-archive")

    include("../cmake/CodeCoverage.cmake")
    APPEND_COVERAGE_COMPILER_FLAGS("-g -O0 -fprofile-arcs -ftest-coverage")
    set(COVERAGE_LCOV_EXCLUDES '/usr/*' '*/.conan/*' '/tests/*' '*/tests/*')
    SETUP_TARGET_FOR_COVERAGE_LCOV(
        NAME coverage                    # New target name
        EXECUTABLE ${EML_TEST_TARGET}
        DEPENDENCIES ${EML_TEST_TARGET}  # Dependencies to build first
    )
endif()

enable_testing()

add_test(UnitTest "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}Test")
