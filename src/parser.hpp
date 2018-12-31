#ifndef EML_PARSER_HPP
#define EML_PARSER_HPP

#include <memory>
#include <optional>
#include <string_view>

#include "error.hpp"
#include "expected.hpp"
#include "memory.hpp"

/**
 * @file parser.hpp
 * @brief A simple [Pratt Parser](https://en.wikipedia.org/wiki/Pratt_parser)
 * for Embedded ML
 */

namespace eml {

struct AstNode;

using ParseResult =
    expected<std::unique_ptr<AstNode>, std::vector<CompilationError>>;

auto parse(std::string_view source, GarbageCollector& gc) -> ParseResult;

} // namespace eml

#endif // EML_PARSER_HPP
