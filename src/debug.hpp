#ifndef EML_DEBUG_HPP
#define EML_DEBUG_HPP

/**
 * @brief This file contains functions useful for eml development
 */

#include <string>

namespace eml {

struct AstNode;

/**
 * @brief The PrintOption enum
 */
enum class AstPrintOption {
  pretty, ///< @brief Pretty print Ast with nice identations
  flat,   ///< @brief Prints Ast into a one line string, useful for unit tests
};

/**
 * @brief Converts AST node into a string with lisp-like prefix notation
 */
[[nodiscard]] auto to_string(const AstNode& node,
                             AstPrintOption = AstPrintOption::pretty)
    -> std::string;

} // namespace eml

#endif // EML_DEBUG_HPP
