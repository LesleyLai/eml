#include <iomanip>
#include <iostream>
#include <sstream>
#include <string_view>

#include "vm.hpp"

namespace eml {

namespace {
#ifdef EML_VM_DEBUG_TRACE_EXECUTION
void printStack(const std::vector<value>& stack)
{
  std::cout << "\nStack: [ ";
  for (const auto& v : stack) {
    std::cout << v << ' ';
  }
  std::cout << "]\n";
}
#endif

// Push value to the stack
void push(std::vector<value>& stack, value value)
{
  stack.push_back(value);
}

// Returns the value to the last
// Warning: Calling pop on a vm with empty stack is undefined.
auto pop(std::vector<value>& stack) -> value
{
  value v = stack.back();
  stack.pop_back();
  return v;
}

// Helper for binary operations
template <typename F> void binary_operation(std::vector<value>& stack, F op)
{
  value right = pop(stack);
  value left = pop(stack);
  push(stack, op(left, right));
}
} // anonymous namespace

auto vm::interpret() -> value
{
  size_t offset = 0;
  for (auto ip = code_.instructions.begin(); ip != code_.instructions.end();
       ++ip) {
#ifdef EML_VM_DEBUG_TRACE_EXECUTION
    printStack(stack_);
    std::cout << code_.disassemble_instruction(ip, offset);
#endif
    const auto instruction = *ip;
    switch (instruction) {
    case op_return:
      return pop(stack_);
    case op_push: {
      ++ip;
      value constant = code_.read_constant(ip);
      push(stack_, constant);
    } break;
    case op_negate:
      push(stack_, -pop(stack_));
      break;
    case op_add:
      binary_operation(stack_, std::plus<value>{});
      break;
    case op_subtract:
      binary_operation(stack_, std::minus<value>{});
      break;
    case op_multiply:
      binary_operation(stack_, std::multiplies<value>{});
      break;
    case op_divide:
      binary_operation(stack_, std::divides<value>{});
      break;
    default:
      std::cerr << "EML Virtual Machine: Unknown instruction " << instruction
                << "!\n";
      std::exit(1);
    }

    ++offset;
  }

  if (stack_.empty()) {
    std::cerr << "Invalid instruction sequences! No value to evaluate to!\n";
    std::exit(1);
  } else {
    return pop(stack_);
  }
}

auto chunk::disassemble_instruction(instruction_iterator ip,
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
  if (offset != 0 && lines[offset].value == lines[offset - 1].value) {
    ss << std::setfill(' ') << std::setw(linum_digits) << '|';
  } else {
    ss << std::setfill('0') << std::setw(linum_digits) << lines[offset].value;
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

} // namespace eml
