#ifndef EML_DEBUG_HPP
#define EML_DEBUG_HPP

#include <string>

namespace eml {

namespace ast {
struct AstNode;
} // namespace ast

/**
 * @brief Converts AST node into a string with lisp-like prefix notation
 */
std::string to_string(const ast::AstNode& node);

} // namespace eml

#endif // EML_DEBUG_HPP
