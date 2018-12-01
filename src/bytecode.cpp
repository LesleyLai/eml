#include <sstream>
#include <string_view>

#include "bytecode.hpp"

namespace eml {

std::string Bytecode::disassemble() const
{
  size_t offset = 0;

  std::string result;

  for (auto ip = instructions.begin(); ip != instructions.end(); ++ip) {
    result += disassemble_instruction(ip, offset);
    const auto instruction = *ip;
    switch (static_cast<opcode>(instruction)) {
    case op_push_f64: {
      ++ip;
    } break;
    default:; // Nothing special
    }

    ++offset;
  }

  return result;
}

auto Bytecode::disassemble_instruction(instruction_iterator ip,
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
  case op_push_f64: {
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
