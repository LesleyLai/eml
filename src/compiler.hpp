#ifndef EML_COMPILER_HPP
#define EML_COMPILER_HPP

#include "ast.hpp"
#include "vm.hpp"

namespace eml {

/**
 * @brief Compiles the AST Expr node expr into bytecode
 */
auto bytecode_from_ast(const ast::Expr& expr) -> chunk;

} // namespace eml

#endif // EML_COMPILER_HPP
