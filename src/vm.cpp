#include <iomanip>
#include <iostream>
#include <sstream>
#include <string_view>

#include "code_generator.hpp"
#include "common.hpp"
#include "eml.hpp"
#include "parser.hpp"

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

auto VM::interpret(const Bytecode& code) -> std::optional<Value>
{
  size_t offset = 0;
  Value result{};

  for (auto ip = code.instructions.begin(); ip != code.instructions.end();
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
      std::cout << code.disassemble_instruction(ip, offset) << '\n';
    }

    const auto instruction = *ip;
    switch (static_cast<opcode>(instruction)) {
    case op_return:
      std::fputs("EML: Do not know how to handle return yet\n", stderr);
      std::exit(-1);
    case op_push_f64: {
      ++ip;
      Value constant = code.read_constant(ip);
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
    return {};
  }
  return pop(stack_);
}

} // namespace eml
