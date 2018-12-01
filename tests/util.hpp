
#ifndef EML_TEST_UTIL_HPP
#define EML_TEST_UTIL_HPP

// Test Utility functions

#include "vm.hpp"

// Push a constant value to vm
inline void push_number(eml::Bytecode& chunk, double value,
                        eml::line_num linum = eml::line_num{0})
{
  chunk.write(eml::op_push_f64, linum);
  const auto offset = chunk.add_constant(eml::Value{value});
  chunk.write(eml::opcode{*offset}, linum);
}

#endif // EML_TEST_UTIL_HPP
