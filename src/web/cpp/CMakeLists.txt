set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(EMCC_LINKER_FLAGS "")

add_executable(eml_web "main.cpp")
target_link_libraries(eml_web eml)

set_target_properties(${PROJECT_NAME}
  PROPERTIES LINK_FLAGS
  "-s 'EXTRA_EXPORTED_RUNTIME_METHODS=[\"ccall\", \"cwrap\"]' ")
