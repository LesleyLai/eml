include(CTest)
set(EML_TEST_TARGET eml-test)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(${EML_TEST_TARGET}
        "expected/assignment.cpp"
        "expected/bases.cpp"
        "expected/constructors.cpp"
        "expected/emplace.cpp"
        "expected/extensions.cpp"
        "expected/issues.cpp"
        "expected/observers.cpp"
        "main.cpp"
        "memory_test.cpp"
        "ast_test.cpp"
        "parser_test.cpp"
        "cast_test.cpp"
        "scanner_test.cpp"
        "string_test.cpp"
        "value_test.cpp"
        "vm_test_util.hpp"
        "vm_test.cpp"
        "type_check_test.cpp"
        integration_test.cpp utils/approval_using_sections_namer.cpp utils/approval_using_sections_namer.hpp)

conan_cmake_run(REQUIRES
        catch2/2.13.2
        approvaltests.cpp/10.3.0
        BASIC_SETUP CMAKE_TARGETS
        BUILD missing)

target_link_libraries(${EML_TEST_TARGET}
        PRIVATE eml CONAN_PKG::catch2 CONAN_PKG::approvaltests.cpp)

if (${EML_BUILD_TESTS_COVERAGE})
    include(ProcessorCount)
    ProcessorCount(PROCESSOR_COUNT)

    set(CMAKE_STATIC_LINKER_FLAGS "--whole-archive")
    set(CMAKE_SHARED_LINKER_FLAGS "--whole-archive")

    include("../cmake/CodeCoverage.cmake")
    APPEND_COVERAGE_COMPILER_FLAGS("-g -O0 -fprofile-arcs -ftest-coverage")
    set(COVERAGE_LCOV_EXCLUDES '/usr/*' '*/.conan/*' '*/tests/*' 'tests/*')
    SETUP_TARGET_FOR_COVERAGE_LCOV(
            NAME coverage                    # New target name
            EXECUTABLE ${EML_TEST_TARGET}
            DEPENDENCIES ${EML_TEST_TARGET}  # Dependencies to build first
    )
endif ()

enable_testing()

add_test(UnitTest "${CMAKE_BINARY_DIR}/bin/${EML_TEST_TARGET}")
