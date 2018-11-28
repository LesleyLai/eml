#include <iomanip>
#include <iostream>
#include <sstream>
#include <string_view>

#include "common.hpp"
#include "vm.hpp"

namespace eml {

namespace {

// Push value to the stack
void push(std::vector<Value>& stack, Value value)
{
  stack.push_back(value);
}

// Returns the value to the last
// Warning: Calling pop on a vm with empty stack is undefined.
auto pop(std::vector<Value>& stack) -> Value
{
  Value v = stack.back();
  stack.pop_back();
  return v;
}

void runtime_error(std::string_view message)
{
  std::clog << "Runtime error: " << message.data() << '\n';
}

// Helper for binary operations
template <typename F> void binary_operation(std::vector<Value>& stack, F op)
{
  Value right = pop(stack);
  Value left = pop(stack);

  if (!left.is_number()) {
    std::stringstream ss;
    ss << "The left operands of a binary operation must be a number.\nGets "
       << left;
    runtime_error(ss.str());
    return;
  }

  if (!right.is_number()) {
    std::stringstream ss;
    ss << "The right operands of a binary operation must be a number.\nGets "
       << right;
    runtime_error(ss.str());
    return;
  }

  push(stack, Value{op(left.unsafe_as_number(), right.unsafe_as_number())});
}

// Helper for comparison operations
template <typename F> void comparison_operation(std::vector<Value>& stack, F op)
{
  Value right = pop(stack);
  Value left = pop(stack);

  push(stack, Value{op(left, right)});
}

} // anonymous namespace

auto VM::interpret() -> Value
{
  size_t offset = 0;
  Value result{};

  for (auto ip = code_.instructions.begin(); ip != code_.instructions.end();
       ++ip) {
    if constexpr (eml::build_options.debug_vm_trace_execution) {
      std::cout << "Stack: [";

      for (auto i = stack_.begin(); i < stack_.end(); ++i) {
        std::cout << to_string(*i, PrintType::no);
        if (i != stack_.end() - 1) {
          std::cout << ", ";
        }
      }

      std::cout << "]\n";
      std::cout << code_.disassemble_instruction(ip, offset) << '\n';
    }

    const auto instruction = *ip;
    switch (static_cast<opcode>(instruction)) {
    case op_return:
      std::fputs("EML: Do not know how to handle return yet\n", stderr);
      std::exit(-1);
    case op_push: {
      ++ip;
      Value constant = code_.read_constant(ip);
      push(stack_, constant);
    } break;
    case op_pop:
      result = pop(stack_);
      break;
    case op_unit:
      push(stack_, Value{});
      break;
    case op_true: {
      push(stack_, Value{true});
    } break;
    case op_false: {
      push(stack_, Value{false});
    } break;
    case op_negate: {
      const Value v = stack_.back();
      if (!v.is_number()) {
        runtime_error("Operand of unary - must be a number.");
        return Value{};
      }
      push(stack_, Value{-pop(stack_).unsafe_as_number()});
    } break;
    case op_not: {
      const Value v = stack_.back();
      if (!v.is_boolean()) {
        runtime_error("Operand of unary ! must be a boolean.");
        return Value{};
      }

      push(stack_, Value{!pop(stack_).unsafe_as_boolean()});
    } break;
    case op_add:
      binary_operation(stack_, std::plus<double>{});
      break;
    case op_subtract:
      binary_operation(stack_, std::minus<double>{});
      break;
    case op_multiply:
      binary_operation(stack_, std::multiplies<double>{});
      break;
    case op_divide:
      binary_operation(stack_, std::divides<double>{});
      break;
    case op_equal:
      comparison_operation(stack_, std::equal_to<Value>{});
      break;
    case op_not_equal:
      comparison_operation(stack_, std::not_equal_to<Value>{});
      break;
    case op_less:
      comparison_operation(stack_, std::less<Value>{});
      break;
    case op_less_equal:
      comparison_operation(stack_, std::less_equal<Value>{});
      break;
    case op_greater:
      comparison_operation(stack_, std::greater<Value>{});
      break;
    case op_greater_equal:
      comparison_operation(stack_, std::greater_equal<Value>{});
      break;
    default:
      EML_UNREACHABLE();
    }

    ++offset;
  }

  if (stack_.empty()) {
    return Value{};
  }
  return pop(stack_);
}

std::string chunk::disassemble() const
{
  size_t offset = 0;

  std::string result;

  for (auto ip = instructions.begin(); ip != instructions.end(); ++ip) {
    result += disassemble_instruction(ip, offset);
    const auto instruction = *ip;
    switch (static_cast<opcode>(instruction)) {
    case op_push: {
      ++ip;
    } break;
    default:; // Nothing special
    }

    ++offset;
  }

  return result;
}

auto chunk::disassemble_instruction(instruction_iterator ip,
                                    std::size_t offset) const -> std::string
{
  std::stringstream ss;

  auto print_hex_dump = [&ss](auto current_ip, std::size_t count) {
    // Over max byte of hex will cause misalignment in output
    constexpr std::size_t max_byte = 5;
    for (auto i = std::size_t{0}; i < count; ++i) {
      const auto code = std::to_integer<unsigned>(*current_ip);
      ss << std::hex << std::setfill('0') << std::setw(2) << code << ' ';
      ++current_ip;
    }
    for (auto i = count; i < max_byte; ++i) {
      ss << "   ";
    }
  };

  auto disassemble_simple_instruction = [&](auto& current_ip,
                                            std::string_view name) {
    print_hex_dump(current_ip, 1);
    ss << name << '\n';
    ++current_ip;
  };

  // Print instruction with one constant argument
  auto disassemble_instruction_with_one_const_parem =
      [&](auto& current_ip, std::string_view name) {
        print_hex_dump(current_ip, 2);
        const auto v = read_constant(++current_ip);
        ss << name << ' ' << to_string(v, PrintType::no) << '\n';
      };

  // Dump file in source line
  constexpr std::size_t linum_digits = 4;
  if (offset != 0 && lines[offset].value == lines[offset - 1].value) {
    ss << std::setfill(' ') << std::setw(linum_digits) << '|';
  } else {
    ss << std::setfill('0') << std::setw(linum_digits) << lines[offset].value;
  }
  ss << "    ";

  switch (static_cast<opcode>(*ip)) {
  case op_return:
    disassemble_simple_instruction(ip, "return");
    break;
  case op_push: {
    disassemble_instruction_with_one_const_parem(ip, "push");
  } break;
  case op_pop:
    disassemble_simple_instruction(ip, "pop");
    break;
  case op_true:
    disassemble_simple_instruction(ip, "true // push true");
    break;
  case op_false:
    disassemble_simple_instruction(ip, "false // push false");
    break;
  case op_unit:
    disassemble_simple_instruction(ip, "unit // push ()");
    break;
  case op_not:
    disassemble_simple_instruction(ip, "not");
    break;
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
  case op_equal:
    disassemble_simple_instruction(ip, "eq // equal to");
    break;
  case op_not_equal:
    disassemble_simple_instruction(ip, "ne // not equal to");
    break;
  case op_less:
    disassemble_simple_instruction(ip, "lt // less than");
    break;
  case op_less_equal:
    disassemble_simple_instruction(ip, "le // less than or equal to");
    break;
  case op_greater:
    disassemble_simple_instruction(ip, "gt // greater than");
    break;
  case op_greater_equal:
    disassemble_simple_instruction(ip, "ge // greater than or equal to");
    break;
  default:
    ss << "Unknown instruction\n";
  }

  return ss.str();
}

} // namespace eml
