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

namespace eml {

struct Value {
  static_assert(std::numeric_limits<double>::is_iec559,
                "Embedded ML require IEEE 754 floating point number is in use");

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

  constexpr Value() noexcept : type{type::Unit} {}
  constexpr explicit Value(double v) noexcept : val{v}, type{type::Number} {}
  constexpr explicit Value(bool b) noexcept : val{b}, type{type::Boolean} {}

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
  case Value::type::Unit:
    return true;
  }

  return false; // Unreachable
}

constexpr auto operator!=(const Value& lhs, const Value& rhs)
{
  return !(lhs == rhs);
}

constexpr auto operator<(const Value& lhs, const Value& rhs)
{
  if (lhs.type != rhs.type) {
    std::clog << "Runtime error: comparing different types\n";
    return false;
  }

  switch (lhs.type) {
  case Value::type::Boolean:
    std::clog << "Runtime error: comparing boolean types\n";
    return false;
  case Value::type::Number:
    return lhs.unsafe_as_number() < rhs.unsafe_as_number();
  case Value::type::Unit:
    std::clog << "Runtime error: comparing unit types\n";
    return false;
  }

  return false; // Unreachable
}

constexpr auto operator<=(const Value& lhs, const Value& rhs)
{
  return (lhs < rhs) || (lhs == rhs);
}

constexpr auto operator>(const Value& lhs, const Value& rhs)
{
  return !(lhs <= rhs);
}

constexpr auto operator>=(const Value& lhs, const Value& rhs)
{
  return !(lhs < rhs);
}

enum PrintType {
  yes,
  no,
};

inline std::string to_string(const Value& v,
                             PrintType print_type = PrintType::yes)
{
  std::stringstream ss;
  switch (v.type) {
  case Value::type::Number:
    ss << v.val.num;
    if (print_type == PrintType::yes) {
      ss << ": Number";
    }
    break;
  case Value::type::Boolean:
    ss << std::boolalpha << v.val.boolean;
    if (print_type == PrintType::yes) {
      ss << ": Bool";
    }
    break;
  case Value::type::Unit:
    ss << "()";
    if (print_type == PrintType::yes) {
      ss << ": Unit";
    }
    break;
  }

  return ss.str();
}

/**
 * @brief Prints value v to the output stream s
 */
inline auto operator<<(std::ostream& s, const Value& v) -> std::ostream&
{
  s << to_string(v);
  return s;
}

} // namespace eml

#endif // EML_VALUE_HPP
