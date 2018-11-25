#include "parser.hpp"
#include "ast.hpp"
#include "common.hpp"
#include "debug.hpp"
#include "scanner.hpp"

#include <cstdint>

namespace eml {

// A error node that represents with syntax error
class ErrorExpr final : public ast::Expr, public ast::FactoryMixin<ErrorExpr> {
  void accept(ast::ExprVisitor&) override
  {
    EML_UNREACHABLE();
  }

  void accept(ast::ExprConstVisitor&) const override
  {
    EML_UNREACHABLE();
  }
};

struct Parser;
auto parse_expression(Parser& parser) -> ast::Expr_ptr;

struct Parser {
  explicit Parser(std::string_view source)
      : scanner{source}, current_itr{scanner.begin()}
  {
    check_unsupported_token_type(*current_itr);
  }

  eml::Scanner scanner;
  eml::Scanner::iterator current_itr;
  eml::Token previous;
  std::vector<CompilationError> errors;

  bool had_error = false;
  bool panic_mode = false; // Ignore errors if in panic

  void check_unsupported_token_type(const eml::Token& token)
  {
    const auto type = token.type;
    switch (type) {
    case token_type::minus_right_arrow:
    case token_type::colon:
    case token_type::semicolon:
    case token_type::greator_greator:
    case token_type::less_less:
      error_at(token, "This operator is reserved by EML language for future "
                      "development, but "
                      "currently the language does not support it");
      break;
    case token_type::keyword_and:
    case token_type::keyword_async:
    case token_type::keyword_await:
    case token_type::keyword_case:
    case token_type::keyword_class:
    case token_type::keyword_def:
    case token_type::keyword_else:
    case token_type::keyword_extern:
    case token_type::keyword_for:
    case token_type::keyword_if:
    case token_type::keyword_let:
    case token_type::keyword_not:
    case token_type::keyword_or:
    case token_type::keyword_return:
    case token_type::keyword_this:
    case token_type::keyword_unsafe:
    case token_type::keyword_variant:
      error_at(token, "This keyword is reserved by EML language for future "
                      "development, but "
                      "currently the language does not support it");
      break;
    default:
      return; // Do nothing
    }
  }

  void consume(const eml::token_type type, const char* message)
  {
    if (current_itr->type == type) {
      advance();
      return;
    }

    error_at(*current_itr, message);
  }

  void error_at(const eml::Token& token, std::string message)
  {
    if (panic_mode) {
      return;
    }
    panic_mode = true;

    errors.emplace_back(std::in_place_type<SyntaxError>, std::move(message),
                        token);
    had_error = true;
  }

  void error_at_previous(const std::string& message)
  {
    error_at(previous, message);
  }

  void advance()
  {
    previous = *current_itr;

    while (true) {
      const auto current = *(++current_itr);
      check_unsupported_token_type(current);
      if (current.type != token_type::error) {
        break;
      }

      // Hits an error token
      error_at(current, std::string(current.text));
    }
  }
};

// clang-format off
/**
 * @page precedence Precedence and Associativity
 * This page shows which expressions have higher precedence, and their
 * associativity.
 *
| Precedence | Operators          | Description                        | Associativity |
| ----:      | :----:             | :----                              | :----         |
| 1          | `.` `()` `[]`      | Grouping, Subscript, Function call | Left          |
| 2          | `!` `-`            | Unary                              | Right         |
| 3          | `*` `/`            | Multiply, Divide                   | Left          |
| 4          | `+` `-`            | Add, Subtract                      | Left          |
| 5          | `<` `>` `<=` `>=`  | Comparison                         | Left          |
| 6          | `==` `!=`          | Equality comparison                | Left          |
| 7          | `and`              | Logical and                        | Left          |
| 8          | `or`               | Logical or                         | Left          |
| 9          | `=`                | Definition, Assignment             | Right         |
 */
// clang-format on
enum Precedence : std::uint8_t {
  prec_none,
  prec_assignment, // =
  prec_or,         // or
  prec_and,        // and
  prec_equality,   // == !=
  prec_comparison, // < > <= >=
  prec_term,       // + -
  prec_factor,     // * /
  prec_unary,      // ! -
  prec_call,       // . () []
  prec_primary
};

using PrefixParselet = ast::Expr_ptr (*)(Parser& Parser);
using InfixParselet = ast::Expr_ptr (*)(Parser& Parser, ast::Expr_ptr left);

struct ParseRule {
  PrefixParselet prefix;
  InfixParselet infix;
  Precedence precedence;
};

constexpr auto get_rule(token_type type) -> ParseRule;

Precedence higher(Precedence p)
{
  return static_cast<Precedence>(
      static_cast<std::underlying_type_t<Precedence>>(p) + 1);
}

auto parse_number(Parser& parser) -> ast::Expr_ptr
{
  const double number = strtod(parser.previous.text.data(), nullptr);
  return ast::LiteralExpr::create(Value{number});
}

auto parse_literal(Parser& parser) -> ast::Expr_ptr
{
  switch (parser.previous.type) {
  case token_type::keyword_unit:
    return ast::LiteralExpr::create(Value{});

  case token_type::keyword_true:
    return ast::LiteralExpr::create(Value{true});

  case token_type::keyword_false:
    return ast::LiteralExpr::create(Value{false});

  default:
    EML_UNREACHABLE();
  }
}

// parses any expression of a given precedence level or higher:
auto parse_precedence(Parser& parser, Precedence precedence) -> ast::Expr_ptr
{
  parser.advance();

  const auto prefix_rule = get_rule(parser.previous.type).prefix;
  if (prefix_rule == nullptr) {
    parser.error_at_previous("expect a prefix operator");
    return ErrorExpr::create();
  }

  auto left_ptr = prefix_rule(parser);

  while (precedence <= get_rule(parser.current_itr->type).precedence) {
    parser.advance();
    const auto infix_rule = get_rule(parser.previous.type).infix;
    if (infix_rule == nullptr) {
      parser.error_at_previous("expect a infix operator");
      return ErrorExpr::create();
    }
    left_ptr = infix_rule(parser, std::move(left_ptr));
  }

  return left_ptr;
}

auto parse_expression(Parser& parser) -> ast::Expr_ptr
{
  return parse_precedence(parser, prec_assignment);
}

auto parse_grouping(Parser& parser)
{
  auto expr_ptr = parse_expression(parser);

  parser.consume(eml::token_type::right_paren,
                 "Expect `)` at the end of the expression");
  return expr_ptr;
}

auto parse_unary(Parser& parser) -> ast::Expr_ptr
{
  const token_type operator_type = parser.previous.type;

  // Compile the operand.
  auto operand_ptr = parse_precedence(parser, prec_unary);

  // Emit the operator instruction.
  switch (operator_type) {
  case token_type::bang:
    return ast::UnaryNotExpr::create(std::move(operand_ptr));
  case token_type::minus:
    return ast::UnaryNegateExpr::create(std::move(operand_ptr));
  default:
    EML_UNREACHABLE();
  }
}

auto parse_binary(Parser& parser, ast::Expr_ptr left_ptr) -> ast::Expr_ptr
{
  // Remember the operator.
  token_type operator_type = parser.previous.type;

  // Compile the right operand.
  const ParseRule rule = get_rule(operator_type);

  auto rhs_ptr = parse_precedence(parser, higher(rule.precedence));

  // Emit the operator instruction.
  switch (operator_type) {
  case token_type::plus:
    return ast::PlusOpExpr::create(std::move(left_ptr), std::move(rhs_ptr));
  case token_type::minus:
    return ast::MinusOpExpr::create(std::move(left_ptr), std::move(rhs_ptr));

  case token_type::star:
    return ast::MultOpExpr::create(std::move(left_ptr), std::move(rhs_ptr));

  case token_type::slash:
    return ast::DivOpExpr::create(std::move(left_ptr), std::move(rhs_ptr));

  case token_type::double_equal:
    return ast::EqOpExpr::create(std::move(left_ptr), std::move(rhs_ptr));

  case token_type::bang_equal:
    return ast::NeqOpExpr::create(std::move(left_ptr), std::move(rhs_ptr));

  case token_type::less:
    return ast::LessOpExpr::create(std::move(left_ptr), std::move(rhs_ptr));

  case token_type::less_equal:
    return ast::LeOpExpr::create(std::move(left_ptr), std::move(rhs_ptr));

  case token_type::greator:
    return ast::GreaterOpExpr::create(std::move(left_ptr), std::move(rhs_ptr));

  case token_type::greater_equal:
    return ast::GeExpr::create(std::move(left_ptr), std::move(rhs_ptr));

  default:
    EML_UNREACHABLE();
  }
}

// Get parse Rules
constexpr auto get_rule(token_type type) -> ParseRule
{
  switch (type) {
#define TOKEN_TABLE_ENTRY(type, type_name, prefix, infix, precedence)          \
  case token_type::type:                                                       \
    return ParseRule{prefix, infix, prec_##precedence};

#include "../src/token_table.inc"
#undef TOKEN_TABLE_ENTRY
  }

  // Unreachable
  return ParseRule{};
}

auto parse(std::string_view source) -> ParseResult
{
  Parser parser{source};
  auto expr = parse_expression(parser);
  parser.consume(token_type::eof, "Expect end of expression");
  if (parser.had_error) {
    return unexpected{std::move(parser.errors)};
  } else {
    if constexpr (build_options.debug_print_ast) {
      std::cout << eml::string_from_ast(*expr) << '\n';
    }
    return std::move(expr);
  }
}

} // namespace eml
