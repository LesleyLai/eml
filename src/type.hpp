#ifndef EML_TYPE_HPP
#define EML_TYPE_HPP

/**
 * @file type.hpp
 * @brief This file stores the types of the Embedded ML values in its type
 * system
 */

#include <iostream>
#include <memory>
#include <variant>

namespace eml {

// clang-format off
struct NumberType {};
struct BoolType {};
struct UnitType {};
struct StringType {};
struct ErrorType {};

// clang-format on

using Type =
    std::variant<NumberType, BoolType, UnitType, StringType, ErrorType>;

auto operator<<(std::ostream& os, const Type& type) -> std::ostream&;

/**
 * @brief Return true if the lhs type match the rhs type
 */
inline auto match(const Type& lhs, const Type& rhs)
{
  return lhs.index() == rhs.index();
}

} // namespace eml

#endif // EML_TYPE_HPP
