set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_executable(eml-cli "main.cpp")
target_link_libraries(eml-cli eml)
set_target_properties(eml-cli PROPERTIES OUTPUT_NAME eml)
