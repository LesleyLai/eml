#ifndef EML_PARSER_HPP
#define EML_PARSER_HPP

#include <memory>
#include <optional>
#include <string_view>

#include "expected.hpp"

/**
 * @file parser.hpp
 * @brief A simple [Pratt Parser](https://en.wikipedia.org/wiki/Pratt_parser)
 * for Embedded ML
 */

namespace eml {

struct SyntaxError {
  std::string_view msg_;

  explicit SyntaxError(std::string_view msg) : msg_{msg} {}
};

namespace ast {
struct Expr;
} // namespace ast

using ParseResult =
    expected<std::unique_ptr<ast::Expr>, std::vector<SyntaxError>>;

auto parse(std::string_view source) -> ParseResult;

} // namespace eml

#endif // EML_PARSER_HPP
