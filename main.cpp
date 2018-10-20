#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

namespace eml {

using value = std::variant<double>;

template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts> overloaded(Ts...)->overloaded<Ts...>;

auto operator<<(std::ostream& s, const value& v) -> std::ostream&
{
  std::visit(overloaded{[&](double d) { s << d; }}, v);
  return s;
}

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
    auto ip = codes_.begin(); // Instruction pointer
    while (ip != codes_.end()) {
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

      ++ip;
    }

    return interpret_result::ok;
  }

private:
  std::vector<opcode> codes_{}; // Codes
  std::vector<value> constants_{};

  auto read_constant(decltype(codes_)::const_iterator ip_) const -> value
  {
    const auto index = static_cast<std::uint32_t>(*ip_);
    return constants_.at(index);
  }
};

auto vm::disassemble() const -> std::string
{
  std::stringstream ss;

  auto print_hex_dump = [&ss](decltype(codes_)::const_iterator ip,
                              std::size_t count) {
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

  auto disassemble_simple_instruction =
      [&](decltype(codes_)::const_iterator& ip, std::string_view name) {
        print_hex_dump(ip, 1);
        ss << name << '\n';
        ++ip;
      };

  // Print instruction with one constant argument
  auto disassemble_instruction_with_one_const_parem =
      [&](decltype(codes_)::const_iterator& ip, std::string_view name) {
        print_hex_dump(ip, 2);
        const auto v = read_constant(++ip);
        ss << name << ' ' << v << '\n';
      };

  for (auto ip = codes_.begin(); ip < codes_.end(); ++ip) {
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
  }
  return ss.str();
}

} // namespace eml

int main()
{
  using namespace eml;
  vm chunk;

  chunk.write(opcode(5));

  chunk.write(op_constant);
  chunk.add_constant(0.5);
  chunk.write(op_constant);
  chunk.add_constant(1.5);
  chunk.write(op_return);

  std::cout << chunk.disassemble() << '\n';

  chunk.interpret();

  return 0;
}
