set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(EMCC_LINKER_FLAGS "")

add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} lib${PROJECT_NAME})

set_target_properties(${PROJECT_NAME}
  PROPERTIES LINK_FLAGS
  "-s 'EXTRA_EXPORTED_RUNTIME_METHODS=[\"ccall\", \"cwrap\"]' ")
