
#ifndef EML_TEST_UTIL_HPP
#define EML_TEST_UTIL_HPP

// Test Utility functions

#include "vm.hpp"

// Push a constant value to vm
inline void push_constant(eml::chunk& chunk, eml::Value value,
                          eml::line_num linum = eml::line_num{0})
{
  chunk.write(eml::op_push, linum);
  const auto offset = chunk.add_constant(value);
  chunk.write(eml::opcode{*offset}, linum);
}

#endif // EML_TEST_UTIL_HPP
