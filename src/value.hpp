#ifndef EML_VALUE_HPP
#define EML_VALUE_HPP

/**
 * @file value.hpp
 * @brief This module contains the representation of a value in the EML VM
 */

#include <iomanip>
#include <iostream>
#include <optional>
#include <type_traits>

namespace eml {

struct value {
  enum class type {
    Unit,
    Boolean,
    Number,
  };

  struct unit_t {
  };

  union val {
    constexpr val() : unit{} {}
    constexpr explicit val(double d) : num{d} {}
    constexpr explicit val(bool b) : boolean{b} {}

    unit_t unit;
    double num;
    bool boolean;
  } val;
  type type;

  constexpr value() noexcept : type{type::Unit} {}
  constexpr explicit value(double v) noexcept : val{v}, type{type::Number} {}
  constexpr explicit value(bool b) noexcept : val{b}, type{type::Boolean} {}

  /**
   * @brief Returns whether the value is a unit value
   */
  constexpr auto is_unit() const noexcept -> bool
  {
    return type == type::Unit;
  }

  /**
   * @brief Returns whether the value is a double
   */
  constexpr auto is_number() const noexcept -> bool
  {
    return type == type::Number;
  }

  /**
   * @brief Returns the value as a number
   * @warning The result is undefined if the value is actually not a double
   */
  constexpr auto unsafe_as_number() const noexcept -> double
  {
    return val.num;
  }

  /**
   * @brief Returns whether the value is a bool
   */
  constexpr auto is_boolean() const noexcept -> bool
  {
    return type == type::Boolean;
  }

  /**
   * @brief Returns the value as a boolean
   * @warning The result is undefined if the value is actually not a boolean
   */
  constexpr auto unsafe_as_boolean() const noexcept -> bool
  {
    return val.boolean;
  }
};

/**
 * @brief Prints value v to the output stream s
 */
inline auto operator<<(std::ostream& s, const value& v) -> std::ostream&
{
  switch (v.type) {
  case value::type::Number:
    s << v.val.num << ": Number";
    break;
  case value::type::Boolean:
    s << std::boolalpha << v.val.boolean << ": Bool";
    break;
  case value::type::Unit:
    s << "(): Unit";
    break;
  }

  return s;
}

} // namespace eml

#endif // EML_VALUE_HPP
