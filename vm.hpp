#ifndef VM_HPP
#define VM_HPP

#include <cassert>
#include <functional>
#include <type_traits>
#include <vector>

#include "value.hpp"

namespace eml {

enum opcode : std::uint8_t {
  op_return,
  op_push, // Pushes a constant to the stack

  // Unary Arithmatics
  op_negate,
  // Binary Arithmatics
  op_add,
  op_subtract,
  op_multiply,
  op_divide,
};

/// @brief Line number
struct line_num {
  std::size_t value;
};

class vm {
public:
  vm()
  {
    constexpr size_t initial_stack_size = 256;
    stack_.reserve(initial_stack_size);
  }
  /**
   * @brief Write an instruction to the vm
   * @param code The instruction
   * @param line The line this instruction in source
   */
  void write(opcode code, line_num line)
  {
    codes_.push_back(code);
    lines_.push_back(line);
  }

  /**
   * @brief Adds a constant value v to the vm
   */
  void add_constant(value v);

  auto interpret() -> value;

private:
  std::vector<opcode> codes_{}; // Codes
  std::vector<value> stack_{};  // Stack of the vm
  std::vector<value> constants_{};
  std::vector<line_num> lines_{}; // Source line information

  auto read_constant(decltype(codes_)::const_iterator ip) const -> value
  {
    const auto index = static_cast<std::underlying_type_t<opcode>>(*ip);
    return constants_.at(index);
  }

  auto disassemble_instruction(decltype(codes_)::const_iterator ip,
                               std::size_t offset) const -> std::string;
};

} // namespace eml

#endif // VM_HPP
