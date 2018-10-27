#include <algorithm>
#include <cassert>
#include <functional>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
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

enum class interpret_result { ok, compile_error, runtime_error };

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

  void add_constant(value v)
  {
    assert(constants_.size() <
           std::numeric_limits<std::underlying_type_t<opcode>>::max());
    codes_.push_back(static_cast<opcode>(constants_.size()));
    constants_.push_back(v);
  }

  auto interpret() -> interpret_result
  {
    size_t offset = 0;
    for (auto ip = codes_.begin(); ip != codes_.end(); ++ip) {
#ifdef EML_VM_DEBUG_TRACE_EXECUTION
      std::cout << "\nStack: [ ";
      for (const auto& v : stack_) {
        std::cout << v << ' ';
      }
      std::cout << "]\n";
      std::cout << disassemble_instruction(ip, offset);
#endif

      const auto instruction = *ip;
      switch (instruction) {
      case op_return:
        std::cout << "Returns: " << pop() << '\n';
        return interpret_result::ok;
      case op_push: {
        ++ip;
        value constant = read_constant(ip);
        push(constant);
      } break;
      case op_negate: {
        try {
          push(-pop());
        }
        catch (const exception::bad_value_type& e) {
          std::cerr << "Embeded ML VM: " << e.what() << '\n';
          return interpret_result::runtime_error;
        }
      } break;
      case op_add:
        binary_operation(std::plus<value>{});
        break;
      case op_subtract:
        binary_operation(std::minus<value>{});
        break;
      case op_multiply:
        binary_operation(std::multiplies<value>{});
        break;
      case op_divide:
        binary_operation(std::divides<value>{});
        break;
      default:
        std::cerr << "EML Virtual Machine: Unknown instruction " << instruction
                  << "!\n";
        return interpret_result::runtime_error;
      }

      ++offset;
    }

    return interpret_result::ok;
  }

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

  // Push value into the stack
  void push(value value);

  // Returns the value to the last
  // Warning: Calling pop on a vm with empty stack is undefined.
  auto pop() -> value;

  // Helper for binary operations
  template <typename F> void binary_operation(F op);
};

auto vm::disassemble_instruction(decltype(codes_)::const_iterator ip,
                                 std::size_t offset) const -> std::string
{
  std::stringstream ss;

  auto print_hex_dump = [&ss](auto ip, std::size_t count) {
    constexpr std::size_t max_byte =
        5; // Over max byte of hex will cause misalignment in output
    for (auto i = std::size_t{0}; i < count; ++i) {
      ss << std::hex << std::setfill('0') << std::setw(2) << *ip << ' ';
      ++ip;
    }
    for (auto i = count; i < max_byte; ++i) {
      ss << "   ";
    }
  };

  auto disassemble_simple_instruction = [&](auto& ip, std::string_view name) {
    print_hex_dump(ip, 1);
    ss << name << '\n';
    ++ip;
  };

  // Print instruction with one constant argument
  auto disassemble_instruction_with_one_const_parem =
      [&](auto& ip, std::string_view name) {
        print_hex_dump(ip, 2);
        const auto v = read_constant(++ip);
        ss << name << ' ' << v << '\n';
      };

  // Dump file in source line
  constexpr std::size_t linum_digits = 4;
  if (offset != 0 && lines_[offset].value == lines_[offset - 1].value) {
    ss << std::setfill(' ') << std::setw(linum_digits) << '|';
  }
  else {
    ss << std::setfill('0') << std::setw(linum_digits) << lines_[offset].value;
  }
  ss << "    ";

  switch (*ip) {
  case op_return:
    disassemble_simple_instruction(ip, "return");
    break;
  case op_push: {
    disassemble_instruction_with_one_const_parem(ip, "push");
  } break;
  case op_negate:
    disassemble_simple_instruction(ip, "negate");
    break;
  case op_add:
    disassemble_simple_instruction(ip, "add");
    break;
  case op_subtract:
    disassemble_simple_instruction(ip, "sub");
    break;
  case op_multiply:
    disassemble_simple_instruction(ip, "mult");
    break;
  case op_divide:
    disassemble_simple_instruction(ip, "div");
    break;
  default:
    ss << "Unknown instruction\n";
  }

  return ss.str();
}

void vm::push(value value) { stack_.push_back(value); }

auto vm::pop() -> value
{
  value v = stack_.back();
  stack_.pop_back();
  return v;
}

template <typename F> void vm::binary_operation(F op)
{
  value right = pop();
  value left = pop();
  push(op(left, right));
}

} // namespace eml

int main()
{
  using namespace eml;
  vm chunk;

  constexpr double first_const = 3, second_const = 1.5, third_const = 2;

  chunk.write(op_push, line_num{0});
  chunk.add_constant(value{first_const});

  chunk.write(op_push, line_num{1});
  chunk.add_constant(value{second_const});

  chunk.write(op_negate, line_num{2});

  chunk.write(op_push, line_num{2});
  chunk.add_constant(value{third_const});

  chunk.write(op_subtract, line_num{2});
  chunk.write(op_divide, line_num{2});

  chunk.write(op_return, line_num{2});

  chunk.interpret();

  return 0;
}
