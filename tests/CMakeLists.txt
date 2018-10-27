add_executable(${PROJECT_NAME}Test "main.cpp" "value_test.cpp" "vm_test.cpp")
target_link_libraries(${PROJECT_NAME}Test lib${PROJECT_NAME} CONAN_PKG::catch2)
