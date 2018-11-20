#ifndef EML_PARSER_HPP
#define EML_PARSER_HPP

#include "vm.hpp"
#include <optional>
#include <string_view>

/**
 * @file parser.hpp
 * @brief A simple [Pratt Parser](https://en.wikipedia.org/wiki/Pratt_parser)
 * for Embedded ML
 */

namespace eml {

/**
 * @brief Compiles source code string into byte-code chunk
 */
std::optional<eml::chunk> compile(std::string_view source);

} // namespace eml

#endif // EML_PARSER_HPP
