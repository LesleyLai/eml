add_executable(${PROJECT_NAME}Test "main.cpp")
target_link_libraries(${PROJECT_NAME}Test CONAN_PKG::catch2)
