#ifndef EML_VALUE_HPP
#define EML_VALUE_HPP

/**
 * @file value.hpp
 * @brief This module contains the representation of a value in the EML VM
 */

#include <iostream>
#include <variant>

namespace eml {

using value = std::variant<double>;

/**
 * @brief Prints value v to the output stream s
 */
auto operator<<(std::ostream& s, const value& v) -> std::ostream&;

} // namespace eml

#endif // EML_VALUE_HPP
