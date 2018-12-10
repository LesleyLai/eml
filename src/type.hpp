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
struct ErrorType {};
// clang-format on

using Type = std::variant<NumberType, BoolType, UnitType, ErrorType>;

std::ostream& operator<<(std::ostream& os, const Type& type);

/**
 * @brief Return true if the lhs type match the rhs type
 */
inline auto match(const Type& lhs, const Type& rhs)
{
  return lhs.index() == rhs.index();
}

[[deprecated("use 'match' instead")]] constexpr auto operator==(const Type& lhs,
                                                                const Type& rhs)
    -> bool
{
  return lhs.index() == rhs.index();
}

} // namespace eml

#endif // EML_TYPE_HPP
