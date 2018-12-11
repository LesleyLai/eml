#ifndef EML_DEBUG_HPP
#define EML_DEBUG_HPP

/**
 * @brief This file contains functions useful for eml development
 */

#include <string>

namespace eml {

namespace ast {
struct AstNode;

/**
 * @brief The PrintOption enum
 */
enum class PrintOption {
  pretty, ///< @brief Pretty print Ast with nice identations
  flat,   ///< @brief Prints Ast into a one line string, useful for unit tests
};

} // namespace ast

/**
 * @brief Converts AST node into a string with lisp-like prefix notation
 */
std::string to_string(const ast::AstNode& node,
                      ast::PrintOption = ast::PrintOption::pretty);

} // namespace eml

#endif // EML_DEBUG_HPP
