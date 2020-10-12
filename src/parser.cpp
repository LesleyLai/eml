#include "parser.hpp"
#include "ast.hpp"
#include "common.hpp"
#include "debug.hpp"
#include "scanner.hpp"
#include "string.hpp"
#include "type.hpp"

#include <cstdint>
#include <vector>

namespace eml {

// A error node that represents with syntax error
class ErrorExpr final : public Expr, public FactoryMixin<ErrorExpr> {
  void accept(AstVisitor& /*visitor*/) override
  {
    EML_UNREACHABLE();
  }

  void accept(AstConstVisitor& /*visitor*/) const override
  {
    EML_UNREACHABLE();
  }
};

struct Parser;
auto parse_toplevel(Parser& parser) -> std::unique_ptr<AstNode>;
auto parse_expression(Parser& parser) -> Expr_ptr;

struct Parser {
  explicit Parser(std::string_view source, GarbageCollector& gc)
      : scanner{source}, current_itr{scanner.begin()}, garbage_collector{gc}
  {
    check_unsupported_token_type(*current_itr);
  }

  eml::Scanner scanner;
  eml::Scanner::iterator current_itr;
  eml::Token previous;
  std::vector<CompilationError> errors;
  std::reference_wrapper<GarbageCollector> garbage_collector;

  bool had_error = false;
  bool panic_mode = false; // Ignore errors if in panic

  void check_unsupported_token_type(const eml::Token& token)
  {
    const auto type = token.type;
    switch (type) {
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
    case token_type::keyword_extern:
    case token_type::keyword_for:
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

  // Check if the current token match a type, produces error otherwise
  void check(const eml::token_type type, const char* message)
  {
    if (current_itr->type == type) {
      return;
    }

    error_at(*current_itr, message);
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
| 4          | `+` `-` `++`       | Add, Subtract, Append              | Left          |
| 5          | `<` `>` `<=` `>=`  | Comparison                         | Left          |
| 6          | `==` `!=`          | Equality comparison                | Left          |
| 7          | `and`              | Logical and                        | Left          |
| 8          | `or`               | Logical or                         | Left          |
| 9          | `\`                | Lambda                             | Right         |
| 10         | `=`                | Definition, Assignment             | Right         |
 */
// clang-format on
enum Precedence : std::uint8_t {
  prec_none,
  prec_assignment, // =
  prec_lambda,     // "\"
  prec_or,         // or
  prec_and,        // and
  prec_equality,   // == !=
  prec_comparison, // < > <= >=
  prec_term,       // + - ++
  prec_factor,     // * /
  prec_unary,      // ! -
  prec_call,       // . () []
  prec_primary
};

using PrefixParselet = Expr_ptr (*)(Parser& Parser);
using InfixParselet = Expr_ptr (*)(Parser& Parser, Expr_ptr left);

struct ParseRule {
  PrefixParselet prefix;
  InfixParselet infix;
  Precedence precedence;
};

constexpr auto get_rule(token_type type) -> ParseRule;

auto higher(Precedence p) -> Precedence
{
  return static_cast<Precedence>(
      static_cast<std::underlying_type_t<Precedence>>(p) + 1);
}

auto parse_block(Parser& parser) -> Expr_ptr
{
  auto expr = parse_expression(parser);

  parser.consume(token_type::right_brace, "A block must end with \'}\'");
  return expr;
}

auto parse_grouping(Parser& parser) -> Expr_ptr
{
  auto expr_ptr = parse_expression(parser);

  parser.consume(eml::token_type::right_paren,
                 "Expect `)` at the end of the expression");

  return expr_ptr;
}

// if else
auto parse_branch(Parser& parser) -> Expr_ptr
{
  parser.consume(eml::token_type::left_paren,
                 "condition of an if expression must in a group");
  auto cond = parse_grouping(parser);

  auto If = parse_expression(parser);

  parser.consume(token_type::keyword_else,
                 "if expression must have an else branch");

  auto Else = parse_expression(parser);

  return IfExpr::create(std::move(cond), std::move(If), std::move(Else));
}

auto parse_number(Parser& parser) -> Expr_ptr
{
  const double number = strtod(parser.previous.text.data(), nullptr);
  return LiteralExpr::create(Value{number}, NumberType{});
}

auto parse_string(Parser& parser) -> Expr_ptr
{
  std::string_view text{parser.previous.text};
  text.remove_prefix(1);
  text.remove_suffix(1);

  auto s_obj = eml::make_string(text, parser.garbage_collector);
  return LiteralExpr::create(Value{s_obj}, StringType{});
}

auto parse_definition(Parser& parser) -> std::unique_ptr<AstNode>
{
  parser.advance();
  const auto id = parser.current_itr->text;
  parser.advance();
  parser.consume(token_type::equal, "Missing equal sign in let");
  auto expr = parse_expression(parser);

  parser.advance();

  return Definition::create(id, std::move(expr));
}

auto parse_identifier(Parser& parser) -> std::unique_ptr<Expr>
{
  return IdentifierExpr::create(std::string{parser.previous.text});
}

auto parse_literal(Parser& parser) -> Expr_ptr
{
  switch (parser.previous.type) {
  case token_type::keyword_unit:
    return LiteralExpr::create(Value{}, UnitType{});

  case token_type::keyword_true:
    return LiteralExpr::create(Value{true}, BoolType{});

  case token_type::keyword_false:
    return LiteralExpr::create(Value{false}, BoolType{});

  default:
    EML_UNREACHABLE();
  }
}

// parses any expression of a given precedence level or higher:
auto parse_precedence(Parser& parser, Precedence precedence) -> Expr_ptr
{
  parser.advance();

  if (parser.previous.type == token_type::error) {
    parser.error_at_previous(std::string{parser.previous.text});
    return ErrorExpr::create();
  }

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

auto parse_toplevel(Parser& parser) -> std::unique_ptr<AstNode>
{
  switch (parser.current_itr->type) {
  case token_type::keyword_let:
    return parse_definition(parser);
  default:
    return parse_expression(parser);
  }
}

auto parse_expression(Parser& parser) -> Expr_ptr
{
  return parse_precedence(parser, prec_assignment);
}

auto parse_lambda(Parser& parser) -> Expr_ptr
{
  std::vector<std::string> args;

  for (; parser.current_itr->type == token_type::identifier; parser.advance()) {
    args.emplace_back(parser.current_itr->text);
  }

  parser.consume(token_type::minus_right_arrow, "A lambda must have ->");

  if (args.empty()) {
    parser.error_at_previous("A lambda should have at least one argument!");
  }

  auto expr_ptr = parse_expression(parser);

  return LambdaExpr::create(std::move(args), std::move(expr_ptr));
}

auto parse_unary(Parser& parser) -> Expr_ptr
{
  const token_type operator_type = parser.previous.type;

  // Compile the operand.
  auto operand_ptr = parse_precedence(parser, prec_unary);

  // Emit the operator instruction.
  switch (operator_type) {
  case token_type::bang:
    return UnaryNotExpr::create(std::move(operand_ptr));
  case token_type::minus:
    return UnaryNegateExpr::create(std::move(operand_ptr));
  default:
    EML_UNREACHABLE();
  }
}

auto parse_binary(Parser& parser, Expr_ptr left_ptr) -> Expr_ptr
{
  // Remember the operator.
  token_type operator_type = parser.previous.type;

  // Compile the right operand.
  const ParseRule rule = get_rule(operator_type);

  auto rhs_ptr = parse_precedence(parser, higher(rule.precedence));

  // Emit the operator instruction.
  switch (operator_type) {
  case token_type::plus:
    return PlusOpExpr::create(std::move(left_ptr), std::move(rhs_ptr));
  case token_type::minus:
    return MinusOpExpr::create(std::move(left_ptr), std::move(rhs_ptr));

  case token_type::star:
    return MultOpExpr::create(std::move(left_ptr), std::move(rhs_ptr));

  case token_type::slash:
    return DivOpExpr::create(std::move(left_ptr), std::move(rhs_ptr));

  case token_type::double_equal:
    return EqOpExpr::create(std::move(left_ptr), std::move(rhs_ptr));

  case token_type::bang_equal:
    return NeqOpExpr::create(std::move(left_ptr), std::move(rhs_ptr));

  case token_type::less:
    return LessOpExpr::create(std::move(left_ptr), std::move(rhs_ptr));

  case token_type::less_equal:
    return LeOpExpr::create(std::move(left_ptr), std::move(rhs_ptr));

  case token_type::greator:
    return GreaterOpExpr::create(std::move(left_ptr), std::move(rhs_ptr));

  case token_type::greater_equal:
    return GeExpr::create(std::move(left_ptr), std::move(rhs_ptr));

  case token_type::plus_plus:
    return AppendOpExpr::create(std::move(left_ptr), std::move(rhs_ptr));

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

[[nodiscard]]
auto parse(std::string_view source, GarbageCollector& gc) -> ParseResult
{
  Parser parser{source, gc};
  auto expr = parse_toplevel(parser);
  parser.consume(token_type::eof, "Expect end of expression");
  if (parser.had_error) {
    return unexpected{std::move(parser.errors)};
  }
  if constexpr (eml::BuildOptions::debug_print_ast) {
    std::cout << eml::to_string(*expr) << '\n';
  }
  return expr;
}

} // namespace eml
