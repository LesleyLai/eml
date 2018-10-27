#ifndef EML_VALUE_HPP
#define EML_VALUE_HPP

/**
 * @file value.hpp
 * @brief This module contains the representation of a value in the EML VM
 */

#include <stdexcept>

namespace eml {

namespace exception {
struct runtime_error : std::runtime_error {
  explicit runtime_error(const char* msg) : std::runtime_error{msg} {}
};

struct bad_value_type : runtime_error {
  explicit bad_value_type(const char* msg) : runtime_error{msg} {}
};
} // namespace exception

struct value {

  union val {
    explicit val(double d) : num{d} {}

    double num;
  } val;

  explicit value(double v) : val{v} {}
};

/**
 * @brief Prints value v to the output stream s
 */
auto operator<<(std::ostream& s, const value& v) -> std::ostream&;

/**
 * @brief Negates the value v if it is a type support negate operation
 * @throw bad_value_type if v does not support negation operation
 */
auto operator-(const value& v) -> value;

/**
 * @brief Adds the value v1 and v2 if they are the type support + operation
 * @throw bad_value_type if v does not support negation operation
 */
auto operator+(const value& v1, const value& v2) -> value;

/**
 * @brief Subtracts the value v1 and v2 if they are the type support - operation
 * @throw bad_value_type if v does not support negation operation
 */
auto operator-(const value& v1, const value& v2) -> value;

/**
 * @brief Multiplies the value v1 and v2 if they are the type support *
 * operation
 * @throw bad_value_type if v does not support negation operation
 */
auto operator*(const value& v1, const value& v2) -> value;

/**
 * @brief Divides the value v1 and v2 if they are the type support / operation
 * @throw bad_value_type if v does not support negation operation
 */
auto operator/(const value& v1, const value& v2) -> value;

} // namespace eml

#endif // EML_VALUE_HPP
