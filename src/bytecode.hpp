#ifndef EML_BYTECODE_HPP
#define EML_BYTECODE_HPP

#include <cstddef>
#include <ostream>
#include <type_traits>
#include <vector>

#include "value.hpp"

/**
 * @file bytecode.hpp This file contains the representation of EML bytecode
 */

namespace eml {

/**
 * @brief The instruction set of the Embedded ML vm
 */
enum opcode : std::underlying_type_t<std::byte> {
  op_return,
  op_push_f64, /*Pushes a float_64 constant with index [arg] to the stack*/
  op_pop,      /*Pops and discards the top value of the stack*/

  op_true,  /*Pushes true to the stack*/
  op_false, /*Pushes false to the stack*/
  op_unit,  /*Pushes unit to the stack*/

  /*Unary Arithmatics*/
  op_negate_f64,
  op_not,

  /*Binary Arithmatics*/
  op_add_f64,
  op_subtract_f64,
  op_multiply_f64,
  op_divide_f64,

  /*String op*/
  op_string_cat, // pop two strings and concatenate them, then push the
                 // result back

  /*Comparisons*/
  op_equal,
  op_not_equal,
  op_less_f64,
  op_less_equal_f64,
  op_greater_f64,
  op_greater_equal_f64,

  /* Jumps */
  op_jmp,       // Unconditionally jump instruction pointer [arg] forward
  op_jmp_false, // Pop and if false then jump the instruction pointer [arg]
                // forward.

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
struct Bytecode {
  std::vector<std::byte> instructions; // Instructions
  std::vector<Value> constants;
  std::vector<line_num> lines; // Source line information

  /**
   * @brief Write an instruction to the instructions
   * @param code The instruction to write
   * @param line The line this instruction in source
   * @return The index of the opcode in instructions
   */
  auto write(opcode code, line_num line) -> std::ptrdiff_t
  {
    instructions.push_back(static_cast<std::byte>(code));
    lines.push_back(line);
    return static_cast<std::ptrdiff_t>(instructions.size() - 1);
  }

  /**
   * @brief Write a byte to the instructions
   * @param code The byte to write
   * @param line The line this instruction in source
   * @return The index of the byte in instructions
   */
  auto write(std::byte code, line_num line) -> std::ptrdiff_t
  {
    instructions.push_back(code);
    lines.push_back(line);
    return static_cast<std::ptrdiff_t>(instructions.size() - 1);
  }

  /**
   * @brief Write a byte to the instructions at a certain index
   * @param code The byte to write
   * @param index The place to write the instruction
   */
  void write_at(std::byte code, std::ptrdiff_t index)
  {
    instructions[static_cast<std::size_t>(index)] = code;
  }

  /**
   * @brief Returns the index of next instruction
   */
  auto next_instruction_index() noexcept -> std::ptrdiff_t
  {
    return static_cast<std::ptrdiff_t>(instructions.size());
  }

  /**
   * @brief Adds a constant value v to the chunk and returns its index
   *
   * Adds a constant value v to the chunk. Returns the index where it was
   * appended so that we can locate that same constant later.
   */
  [[nodiscard]] std::optional<opcode_num_type> add_constant(Value v)
  {
    if (constants.size() >= std::numeric_limits<opcode_num_type>::max()) {
      return {};
    }
    constants.push_back(v);
    return static_cast<opcode_num_type>(constants.size() - 1);
  }

  auto disassemble() const -> std::string;

  friend std::ostream& operator<<(std::ostream& os, const Bytecode& bytecode);

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

} // namespace eml

#endif // EML_BYTECODE_HPP
