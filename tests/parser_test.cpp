#include "../src/scanner.hpp"
#include "../src/vm.hpp"

/**
 * @file parser.hpp
 * @brief A simple <a href="https://en.wikipedia.org/wiki/Pratt_parser">Pratt
 * Parser</a> for Embeded ML
 */

namespace eml {

chunk parse(std::string_view source);

class parser {
public:
  parser(std::string_view source, eml::chunk& current_chunk)
      : scanner_{source}, compiling_chunk_{&current_chunk},
        current_itr_{scanner_.begin()}
  {
    advance();
  }

  void parser_expression()
  {

    emit_code(op_return);
  }

  void emit_code(eml::opcode code)
  {
    compiling_chunk_->write(code, eml::line_num{previous_.line});
  }

  void emit_codes(eml::opcode code1, eml::opcode code2)
  {
    emit_code(code1);
    emit_code(code2);
  }

  const token& previous() const
  {
    return previous_;
  }

  chunk& compiling_chunk() const
  {
    return *compiling_chunk_;
  }

  void error_at(const token& token, std::string_view message)
  {
    if (panic_mode) {
      return;
    }
    panic_mode = true;

    std::clog << "L" << token.line << ':' << token.column << " Error";

    switch (token.type) {
    case token_type::eof:
      fputs(" at end", stderr);
      break;
    case token_type::error:
      // Nothing
      break;
    default:
      std::clog << " at \"" << token.text << "\"";
    }

    std::clog << ": " << message << '\n';
    had_error = true;
  }

  void error_at_previous(std::string_view message)
  {
    error_at(previous_, message);
  }

private:
  scanner scanner_;
  chunk* compiling_chunk_; // Should never be nullptr
  scanner::iterator current_itr_;
  token previous_;

  bool had_error = false;
  bool panic_mode = false; // Ignore errors if in panic

  void advance()
  {
    previous_ = *current_itr_;

    while (true) {
      const auto current = *(++current_itr_);
      if (current.type != token_type::error) {
        break;
      }

      error_at(current, current.text);
    }
  }
};

void parse_number(parser& parser)
{
  double number = strtod(parser.previous().text.data(), nullptr);
  auto& chunk = parser.compiling_chunk();
  const auto offset = chunk.add_constant(eml::value{number});
  if (!offset) {
    parser.error_at_previous("EML: Too many constants in one chunk.");
    return;
  }

  parser.emit_code(eml::op_push);
  parser.emit_code(eml::opcode{*offset});
}

chunk compile(std::string_view source)
{
  chunk output_chunk;
  parser parser{source, output_chunk};
  parser.parser_expression();
  return output_chunk;
}

} // namespace eml

#include <catch2/catch.hpp>
TEST_CASE("Test parsing")
{
  const auto result = eml::compile("1 + 2 + 3 sdf@@@@@@  4");
  std::cout << result.disassemble();
}