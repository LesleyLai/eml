#ifndef EML_VALUE_HPP
#define EML_VALUE_HPP

/**
 * @file value.hpp
 * @brief This module contains the representation of a value in the EML VM
 */

#include <iostream>
#include <optional>
#include <type_traits>

namespace eml {

struct value {
  enum class type {
    Unit,
    Number,
  };

  struct unit_t {
  };

  union val {
    constexpr val() : unit{} {}
    constexpr explicit val(double d) : num{d} {}

    unit_t unit;
    double num;
  } val;
  type type;

  constexpr value() noexcept : type{type::Unit} {}
  constexpr explicit value(double v) noexcept : val{v}, type{type::Number} {}

  /**
   * @brief Returns the value v as a double if it exist, nullopt otherwise
   */
  template <typename T> constexpr auto as() const -> std::optional<T>;
};

template <> constexpr auto value::as<double>() const -> std::optional<double>
{
  {
    if (type == value::type::Number) {
      return val.num;
    }

    return {};
  }
}

/**
 * @brief Prints value v to the output stream s
 */
inline auto operator<<(std::ostream& s, const value& v) -> std::ostream&
{
  switch (v.type) {
  case value::type::Number:
    s << v.val.num;
    break;
  case value::type::Unit:
    s << "Unit";
    break;
  }

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
