#ifndef EML_VALUE_HPP
#define EML_VALUE_HPP

/**
 * @file value.hpp
 * @brief This module contains the representation of a value in the EML VM
 */

#include <iostream>

namespace eml {

struct value {
  union val {
    constexpr explicit val(double d) : num{d} {}

    double num;
  } val;

  constexpr explicit value(double v) noexcept : val{v} {}
};

constexpr auto as_double(value v) -> double
{
  return v.val.num;
}

/**
 * @brief Prints value v to the output stream s
 */
inline auto operator<<(std::ostream& s, const value& v) -> std::ostream&
{
  s << v.val.num;
  return s;
}

/**
 * @brief Negates the value v if it is a type support negate operation
 */
constexpr auto operator-(const value& v) noexcept -> value
{
  return eml::value{-v.val.num};
}

/**
 * @brief Adds the value v1 and v2 if they are the type support + operation
 */
constexpr auto operator+(const value& v1, const value& v2) noexcept -> value
{
  return eml::value{v1.val.num + v2.val.num};
}

/**
 * @brief Subtracts the value v1 and v2 if they are the type support - operation
 */
constexpr auto operator-(const value& v1, const value& v2) noexcept -> value
{
  return eml::value{v1.val.num - v2.val.num};
}

/**
 * @brief Multiplies the value v1 and v2 if they are the type support *
 * operation
 */
constexpr auto operator*(const value& v1, const value& v2) noexcept -> value
{
  return eml::value{v1.val.num * v2.val.num};
}

/**
 * @brief Divides the value v1 and v2 if they are the type support / operation
 */
constexpr auto operator/(const value& v1, const value& v2) noexcept -> value
{
  return eml::value{v1.val.num / v2.val.num};
}

} // namespace eml

#endif // EML_VALUE_HPP
