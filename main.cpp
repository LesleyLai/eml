#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
#include <vector>

#include "value.hpp"

namespace eml {

enum opcode : std::uint8_t {
  op_return = 0,
  op_constant = 1,
};

/// @brief Line number
struct line_num {
  std::size_t value;
};

enum class interpret_result { ok, compile_error, runtime_error };

class vm {
public:
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
    constants_.push_back(std::move(v));
  }

  auto disassemble() const -> std::string;

  auto interpret() -> interpret_result
  {
    // Instruction pointer
    for (auto ip = codes_.begin(); ip != codes_.end(); ++ip) {
#ifdef EML_VM_DEBUG_TRACE_EXECUTION
      std::cout << disassemble_instruction(ip);
#endif

      const auto instruction = *ip;
      switch (instruction) {
      case op_return:
        return interpret_result::ok;
      case op_constant: {
        ++ip;
        value constant = read_constant(ip);
        std::cout << constant << '\n';
      } break;
      default:
        std::cerr << "EML Virtual Machine: Unknown instruction " << instruction
                  << "!\n";
        return interpret_result::runtime_error;
      }
    }

    return interpret_result::ok;
  }

private:
  std::vector<opcode> codes_{}; // Codes
  std::vector<value> constants_{};
  std::vector<line_num> lines_{}; // Source line information

  auto read_constant(decltype(codes_)::const_iterator ip) const -> value
  {
    const auto index = static_cast<std::underlying_type_t<opcode>>(*ip);
    return constants_.at(index);
  }

  auto disassemble_instruction(decltype(codes_)::const_iterator ip) const
      -> std::string;
};

auto vm::disassemble() const -> std::string
{
  std::stringstream ss;

  for (auto ip = codes_.begin(); ip < codes_.end(); ++ip) {
    ss << disassemble_instruction(ip);
  }
  return ss.str();
}

auto vm::disassemble_instruction(decltype(codes_)::const_iterator ip) const
    -> std::string
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

  std::size_t offset = ip - codes_.begin();

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
  case op_constant: {
    disassemble_instruction_with_one_const_parem(ip, "const");
  } break;
  default:
    ss << "Unknown instruction\n";
  }

  return ss.str();
}

} // namespace eml

int main()
{
  using namespace eml;
  vm chunk;

  constexpr double first_const = 0.5, second_const = 1.5;

  chunk.write(op_constant, line_num{0});
  chunk.add_constant(first_const);
  chunk.write(op_constant, line_num{0});
  chunk.add_constant(second_const);
  chunk.write(op_return, line_num{1});

  // std::cout << chunk.disassemble() << '\n';

  chunk.interpret();

  return 0;
}
