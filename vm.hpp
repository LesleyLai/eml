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

class vm;

/**
 * @brief A chunk of eml bytecode
 */
struct chunk {
  std::vector<opcode> instructions; // Instructions
  std::vector<value> constants;
  std::vector<line_num> lines; // Source line information

  /**
   * @brief Write an instruction to the chunk
   * @param code The instruction
   * @param line The line this instruction in source
   */
  void write(opcode code, line_num line)
  {
    instructions.push_back(code);
    lines.push_back(line);
  }

  /**
   * @brief Adds a constant value v to the chunk
   *
   * Add a constant value v to the chunk
   */
  void add_constant(value v)
  {
    assert(constants.size() <
           std::numeric_limits<std::underlying_type_t<opcode>>::max());
    instructions.push_back(static_cast<opcode>(constants.size()));
    constants.push_back(v);
  }

private:
  friend vm;
  using instruction_iterator = decltype(instructions)::const_iterator;
  auto read_constant(const instruction_iterator& ip) const -> value
  {
    const auto index = static_cast<std::underlying_type_t<opcode>>(*ip);
    return constants.at(index);
  }

  auto disassemble_instruction(instruction_iterator ip,
                               std::size_t offset) const -> std::string;
};

class vm {
public:
  explicit vm(chunk code) : code_{std::move(code)}
  {
    constexpr size_t initial_stack_size = 256;
    stack_.reserve(initial_stack_size);
  }

  auto interpret() -> value;

private:
  chunk code_;
  std::vector<value> stack_{}; // Stack of the vm
};

} // namespace eml

#endif // VM_HPP
