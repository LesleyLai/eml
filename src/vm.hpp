#ifndef EML_VM_HPP
#define EML_VM_HPP

#include <functional>
#include <type_traits>
#include <vector>

#include "value.hpp"

namespace eml {

/**
 * @brief The instruction set of the Embedded ML vm
 */
enum opcode : std::uint8_t {
  op_return,
  op_push, // Pushes a constant to the stack
  op_pop,  // Pop a constant from the stack

  op_true,  // Pushes true to the stack
  op_false, // Pushes false to the stack
  op_unit,  // Pushes unit to the stack

  // Unary Arithmatics
  op_negate,
  op_not,

  // Binary Arithmatics
  op_add,
  op_subtract,
  op_multiply,
  op_divide,

  // Comparisons
  op_equal,
  op_not_equal,
  op_less,
  op_less_equal,
  op_greater,
  op_greater_equal,
};

/// @brief The underlying numerical type of the @ref opcode enum
using opcode_num_type = std::underlying_type_t<opcode>;

/// @brief Line number
struct line_num {
  std::size_t value;
};

class VM;

/**
 * @brief A chunk of eml bytecode
 */
struct chunk {
  std::vector<opcode> instructions; // Instructions
  std::vector<Value> constants;
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
   * @brief Adds a constant value v to the chunk and returns its index
   *
   * Adds a constant value v to the chunk. Returns the index where it was
   * appended so that we can locate that same constant later.
   */
  [[nodiscard]] std::optional<opcode_num_type> add_constant(Value v) {
    if (constants.size() >= std::numeric_limits<opcode_num_type>::max()) {
      return {};
    }
    constants.push_back(v);
    return static_cast<opcode_num_type>(constants.size() - 1);
  }

  auto disassemble() const -> std::string;

private:
  friend VM;
  using instruction_iterator = decltype(instructions)::const_iterator;
  auto read_constant(const instruction_iterator& ip) const -> Value
  {
    const auto index = static_cast<std::underlying_type_t<opcode>>(*ip);
    return constants.at(index);
  }

  auto disassemble_instruction(instruction_iterator ip,
                               std::size_t offset) const -> std::string;
};

class VM {
public:
  explicit VM(chunk code) : code_{std::move(code)}
  {
    constexpr size_t initial_stack_size = 256;
    stack_.reserve(initial_stack_size);
  }

  auto interpret() -> Value;

private:
  chunk code_;
  std::vector<Value> stack_{}; // Stack of the vm
};

} // namespace eml

#endif // EML_VM_HPP
