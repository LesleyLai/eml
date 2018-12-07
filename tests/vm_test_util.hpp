#ifndef EML_VM_TEST_UTIL_HPP
#define EML_VM_TEST_UTIL_HPP

// Test Utility functions

#include "vm.hpp"

// Write an instruction to vm
inline void write_jump(eml::Bytecode& chunk, eml::opcode instruction,
                       std::underlying_type_t<eml::opcode> amount,
                       eml::line_num linum = eml::line_num{0})
{
  chunk.write(instruction, linum);
  chunk.write(eml::opcode{amount}, linum);
}

// Write an instruction to vm
inline void write_instruction(eml::Bytecode& chunk, eml::opcode instruction,
                              eml::line_num linum = eml::line_num{0})
{
  chunk.write(instruction, linum);
}

// Push a constant number to vm
inline void push_number(eml::Bytecode& chunk, double value,
                        eml::line_num linum = eml::line_num{0})
{
  chunk.write(eml::op_push_f64, linum);
  const auto offset = chunk.add_constant(eml::Value{value});
  chunk.write(eml::opcode{*offset}, linum);
}

#endif // EML_VM_TEST_UTIL_HPP
