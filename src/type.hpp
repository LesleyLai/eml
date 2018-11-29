#ifndef EML_TYPE_HPP
#define EML_TYPE_HPP

/**
 * @file type.hpp
 * @brief This file stores the types of the Embedded ML values in its type
 * system
 */

#include <iostream>
#include <variant>

namespace eml {

// clang-format off
struct NumberType {};
struct BoolType {};
struct UnitType {};
// clang-format on

using Type = std::variant<NumberType, BoolType, UnitType>;

std::ostream& operator<<(std::ostream& os, const Type& type);

constexpr auto operator==(const Type& lhs, const Type& rhs) -> bool
{
  return lhs.index() == rhs.index();
}

constexpr auto operator!=(const Type& lhs, const Type& rhs) -> bool
{
  return !(lhs == rhs);
}

} // namespace eml

#endif // EML_TYPE_HPP
