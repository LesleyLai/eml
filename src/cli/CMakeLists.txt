if(EMSCRIPTEN)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

    add_executable(eml-cli "web/main.cpp")
    target_link_libraries(eml-cli eml)
    set_target_properties(eml-cli PROPERTIES OUTPUT_NAME eml)

    message("Build for emscripten")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(EMCC_LINKER_FLAGS "")
    set_target_properties(eml-cli
      PROPERTIES LINK_FLAGS
      "-s 'EXTRA_EXPORTED_RUNTIME_METHODS=[\"ccall\", \"cwrap\"]' ")
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

    add_executable(eml-cli "main.cpp")
    target_link_libraries(eml-cli eml)
    set_target_properties(eml-cli PROPERTIES OUTPUT_NAME eml)
endif()
