#include <utility>

#ifndef EML_ERROR_HPP
#define EML_ERROR_HPP

/**
 * @file error.hpp
 * @brief This file contains the definition of compile time errors in eml
 */

#include <string>
#include <variant>

#include "common.hpp"
#include "scanner.hpp"

namespace eml {

struct SyntaxError {
  std::string msg;
  Token at;

  SyntaxError(std::string msg_in, Token token)
      : msg{std::move(msg_in)}, at{token}
  {
  }
};

struct TypeError {
  std::string msg;

  explicit TypeError(std::string msg_in) : msg{std::move(msg_in)} {}
};

using CompilationError = std::variant<SyntaxError, TypeError>;

std::string to_string(const CompilationError& error);

} // namespace eml

#endif // EML_ERROR_HPP
