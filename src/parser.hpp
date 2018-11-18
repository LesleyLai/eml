#ifndef EML_PARSER_HPP
#define EML_PARSER_HPP

#include "vm.hpp"
#include <string_view>

/**
 * @file parser.hpp
 * @brief A simple <a href="https://en.wikipedia.org/wiki/Pratt_parser">Pratt
 * Parser</a> for Embeded ML
 */

namespace eml {

/**
 * @brief Compiles source code string into byte-code chunk
 */
eml::chunk compile(std::string_view source);

} // namespace eml

#endif // EML_PARSER_HPP
