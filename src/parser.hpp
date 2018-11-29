#ifndef EML_PARSER_HPP
#define EML_PARSER_HPP

#include <memory>
#include <optional>
#include <string_view>

#include "error.hpp"
#include "expected.hpp"

/**
 * @file parser.hpp
 * @brief A simple [Pratt Parser](https://en.wikipedia.org/wiki/Pratt_parser)
 * for Embedded ML
 */

namespace eml {

namespace ast {
struct AstNode;
} // namespace ast

using ParseResult =
    expected<std::unique_ptr<ast::AstNode>, std::vector<CompilationError>>;

auto parse(std::string_view source) -> ParseResult;

} // namespace eml

#endif // EML_PARSER_HPP
