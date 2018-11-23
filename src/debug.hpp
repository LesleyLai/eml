#ifndef EML_DEBUG_HPP
#define EML_DEBUG_HPP

#include <string>

namespace eml {

namespace ast {
struct Expr;
} // namespace ast

/**
 * @brief Converts AST node into a string with lisp-like prefix notation
 */
std::string string_from_ast(const ast::Expr& expr);

} // namespace eml

#endif // EML_DEBUG_HPP
