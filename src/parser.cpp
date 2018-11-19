#include "parser.hpp"
#include "scanner.hpp"

#include <cstdint>

namespace eml {

struct parser;
void parse_expression(parser& parser);

struct parser {
  parser(std::string_view source, eml::chunk& current_chunk)
      : scanner{source}, compiling_chunk{&current_chunk}, current_itr{
                                                              scanner.begin()}
  {
    parse_expression(*this);
    consume(token_type::eof, "Expect end of expression");
  }

  eml::scanner scanner;
  eml::chunk* compiling_chunk; // Should never be nullptr
  eml::scanner::iterator current_itr;
  eml::token previous;

  bool had_error = false;
  bool panic_mode = false; // Ignore errors if in panic

  void emit_code(eml::opcode code)
  {
    compiling_chunk->write(code, eml::line_num{previous.line});
  }

  void consume(const eml::token_type type, const char* message)
  {
    if (current_itr->type == type) {
      advance();
      return;
    }

    error_at(*current_itr, message);
  }

  void error_at(const eml::token& token, std::string_view message)
  {
    if (panic_mode) {
      return;
    }
    panic_mode = true;

    std::clog << "L" << token.line << ':' << token.column << " Syntax Error";

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

    std::clog << ": " << message << std::endl;
    had_error = true;
  }

  void error_at_previous(std::string_view message)
  {
    error_at(previous, message);
  }

  void advance()
  {
    previous = *current_itr;

    while (true) {
      const auto current = *(++current_itr);
      if (current.type != token_type::error) {
        break;
      }

      error_at(current, current.text);
    }
  }
};

enum Precedence : std::uint8_t {
  prec_none,
  prec_assignment, // =
  prec_or,         // or
  prec_and,        // and
  prec_equality,   // == !=
  prec_comparison, // < > <= >=
  prec_term,       // + -
  prec_factor,     // * /
  prec_unary,      // ! - +
  prec_call,       // . () []
  prec_primary
};

using ParserFn = void (*)(parser& parser);

struct ParseRule {
  ParserFn prefix;
  ParserFn infix;
  Precedence precedence;
};

constexpr auto get_rule(token_type type) -> ParseRule;

Precedence higher(Precedence p)
{
  return static_cast<Precedence>(
      static_cast<std::underlying_type_t<Precedence>>(p) + 1);
}

void parse_number(parser& parser)
{
  const double number = strtod(parser.previous.text.data(), nullptr);
  const auto offset = parser.compiling_chunk->add_constant(eml::value{number});
  if (!offset) {
    parser.error_at_previous("EML: Too many constants in one chunk.");
    return;
  }

  parser.emit_code(eml::op_push);
  parser.emit_code(eml::opcode{*offset});
}

void parse_literal(parser& parser)
{
  switch (parser.previous.type) {
  case token_type::keyword_true:
    parser.emit_code(eml::op_true);
    break;
  case token_type::keyword_false:
    parser.emit_code(eml::op_false);
    break;
  default:
    return; // Unreachable.
  }
}

// parses any expression of a given precedence level or higher:
void parse_precedence(parser& parser, Precedence precedence)
{

  parser.advance();

  const auto prefix_rule = get_rule(parser.previous.type).prefix;
  if (prefix_rule == nullptr) {
    parser.error_at_previous("expect a prefix operator");
    return;
  }

  prefix_rule(parser);

  while (precedence <= get_rule(parser.current_itr->type).precedence) {
    parser.advance();
    const auto infix_rule = get_rule(parser.previous.type).infix;
    if (infix_rule == nullptr) {
      parser.error_at_previous("expect a infix operator");
      return;
    }
    infix_rule(parser);
  }
}

void parse_expression(parser& parser)
{
  parse_precedence(parser, prec_assignment);
}

void parse_grouping(parser& parser)
{
  parse_expression(parser);
  parser.consume(eml::token_type::right_paren,
                 "Expect `)` at the end of the expression");
}

void parse_unary(parser& parser)
{
  const token_type operator_type = parser.previous.type;

  // Compile the operand.
  parse_precedence(parser, prec_unary);

  // Emit the operator instruction.
  switch (operator_type) {
  case token_type::bang:
    parser.emit_code(eml::op_not);
    break;
  case token_type::minus:
    parser.emit_code(eml::op_negate);
    break;
  default:
    std::clog << "Unsupported token " << parser.previous << '\n';
    std::exit(1);
  }
}

void parse_binary(parser& parser)
{
  // Remember the operator.
  token_type operator_type = parser.previous.type;

  // Compile the right operand.
  const ParseRule rule = get_rule(operator_type);
  parse_precedence(parser, higher(rule.precedence));

  // Emit the operator instruction.
  switch (operator_type) {
  case token_type::plus:
    parser.emit_code(op_add);
    break;
  case token_type::minus:
    parser.emit_code(op_subtract);
    break;
  case token_type::star:
    parser.emit_code(op_multiply);
    break;
  case token_type::slash:
    parser.emit_code(op_divide);
    break;
  default:
    std::clog << "Unsupported token " << parser.previous << '\n';
    std::exit(-1);
  }
}

// Get parse Rules
constexpr auto get_rule(token_type type) -> ParseRule
{
  switch (type) {
#define TOKEN_TABLE_ENTRY(type, type_name, prefix, infix, precedence)          \
  case token_type::type:                                                       \
    return ParseRule{prefix, infix, prec_##precedence};

#include "../src/token_table.impl"
#undef TOKEN_TABLE_ENTRY
  }

  // Unreachable
  return ParseRule{};
}

chunk compile(std::string_view source)
{
  chunk output_chunk;
  parser parser{source, output_chunk};
  if (parser.had_error) {
    return chunk{};
  }
  return output_chunk;
}

} // namespace eml
