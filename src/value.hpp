#ifndef EML_VALUE_HPP
#define EML_VALUE_HPP

/**
 * @file value.hpp
 * @brief This module contains the representation of a value in the EML VM
 */

#include <iomanip>
#include <iostream>
#include <limits>
#include <optional>
#include <type_traits>

#include "common.hpp"
#include "object.hpp"

namespace eml {

struct Value {
  static_assert(std::numeric_limits<double>::is_iec559,
                "Embedded ML require IEEE 754 floating point number is in use");

  enum class type {
    Unit,
    Boolean,
    Number,
    Reference,
  };

  struct unit_t {
  };

  union val {
    constexpr val() : unit{} {}
    constexpr explicit val(double d) : num{d} {}
    constexpr explicit val(bool b) : boolean{b} {}
    constexpr explicit val(Obj* o) : ref{o} {}

    unit_t unit;
    double num;
    bool boolean;
    Obj* ref;
  } val;
  type type;

  constexpr Value() noexcept : type{type::Unit} {}
  constexpr explicit Value(double v) noexcept : val{v}, type{type::Number} {}
  constexpr explicit Value(bool b) noexcept : val{b}, type{type::Boolean} {}
  constexpr explicit Value(Obj* o) noexcept : val{o}, type{type::Reference} {}

  constexpr Value(const Value& value) noexcept = default;
  Value& operator=(const Value& value) noexcept = default;

  constexpr Value(Value&& value) noexcept = default;
  Value& operator=(Value&& value) noexcept = default;
  ~Value() = default;

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

  /**
   * @brief Returns whether the value is a reference
   */
  constexpr auto is_reference() const noexcept -> bool
  {
    return type == type::Reference;
  }

  /**
   * @brief Extracts the underlying reference from the Value
   * @warning The result is undefined if the value is actually not a reference
   */
  constexpr auto unsafe_as_reference() const noexcept -> Obj*
  {
    return val.ref;
  }
};

constexpr auto operator==(const Value& lhs, const Value& rhs)
{
  if (lhs.type != rhs.type) {
    std::clog << "Runtime error: equality test on different types\n";
    return false;
  }

  switch (lhs.type) {
  case Value::type::Boolean:
    return lhs.unsafe_as_boolean() == rhs.unsafe_as_boolean();
  case Value::type::Number:
    return lhs.unsafe_as_number() == rhs.unsafe_as_number();
  case Value::type::Reference:
    return lhs.unsafe_as_reference() == rhs.unsafe_as_reference();
  case Value::type::Unit:
    return true;
  }

  return false; // Unreachable
}

constexpr auto operator!=(const Value& lhs, const Value& rhs)
{
  return !(lhs == rhs);
}

auto to_string(const Value& v, PrintType print_type = PrintType::yes)
    -> std::string;

/**
 * @brief Prints value v to the output stream s
 */
auto operator<<(std::ostream& s, const Value& v) -> std::ostream&;

} // namespace eml

#endif // EML_VALUE_HPP
