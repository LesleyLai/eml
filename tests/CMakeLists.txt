add_executable(${PROJECT_NAME}Test
    "expected/assignment.cpp"
    "expected/bases.cpp"
    "expected/constructors.cpp"
    "expected/emplace.cpp"
    "expected/extensions.cpp"
    "expected/issues.cpp"
    "expected/observers.cpp"
    "main.cpp"
    "ast_test.cpp"
    "compiler_test.cpp"
    "parser_test.cpp"
    "scanner_test.cpp"
    "util.hpp"
    "value_test.cpp"
    "vm_test.cpp"
    )
target_link_libraries(${PROJECT_NAME}Test lib${PROJECT_NAME} CONAN_PKG::catch2)
