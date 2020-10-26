add_library(eml_disassembler
        eml_disassembler.cpp
        eml_disassembler.hpp)

target_link_libraries(eml_disassembler PUBLIC eml)
add_library(eml::disassembler ALIAS eml_disassembler)
target_include_directories(eml_disassembler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
