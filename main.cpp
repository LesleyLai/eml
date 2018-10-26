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

enum opcode : std::uint32_t {
  op_return = 0,
  op_constant = 1,
};

enum class interpret_result { ok, compile_error, runtime_error };

class vm {
public:
  void write(opcode code) { codes_.push_back(code); }

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
    const auto size = codes_.size();

    // Instruction pointer
    for (auto ip = std::size_t{0}; ip < size; ++ip) {
      const auto instruction = codes_[ip];
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

  auto read_constant(std::size_t ip) const -> value
  {
    const auto index = static_cast<std::underlying_type_t<opcode>>(codes_[ip]);
    return constants_.at(index);
  }
};

auto vm::disassemble() const -> std::string
{
  std::stringstream ss;

  auto print_hex_dump = [&ss, this](std::size_t ip, std::size_t count) {
    constexpr std::size_t max_byte =
        5; // Over max byte of hex will cause misalignment in output
    for (auto i = std::size_t{0}; i < count; ++i) {
      ss << std::hex << std::setfill('0') << std::setw(2) << codes_[ip] << ' ';
      ++ip;
    }
    for (auto i = count; i < max_byte; ++i) {
      ss << "   ";
    }
  };

  auto disassemble_simple_instruction = [&](std::size_t& ip,
                                            std::string_view name) {
    print_hex_dump(ip, 1);
    ss << name << '\n';
    ++ip;
  };

  // Print instruction with one constant argument
  auto disassemble_instruction_with_one_const_parem =
      [&](std::size_t& ip, std::string_view name) {
        print_hex_dump(ip, 2);
        const auto v = read_constant(++ip);
        ss << name << ' ' << v << '\n';
      };

  for (auto ip = std::size_t{0}, size = codes_.size(); ip < size; ++ip) {
    switch (codes_[ip]) {
    case op_return:
      disassemble_simple_instruction(ip, "return");
      break;
    case op_constant: {
      disassemble_instruction_with_one_const_parem(ip, "const");
    } break;
    default:
      ss << "Unknown instruction\n";
    }
  }
  return ss.str();
}

} // namespace eml

int main()
{
  using namespace eml;
  vm chunk;

  constexpr double first_const = 0.5, second_const = 1.5;

  chunk.write(op_constant);
  chunk.add_constant(first_const);
  chunk.write(op_constant);
  chunk.add_constant(second_const);
  chunk.write(op_return);

  std::cout << chunk.disassemble() << '\n';

  chunk.interpret();

  return 0;
}
