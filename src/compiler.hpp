#ifndef EML_COMPILER_HPP
#define EML_COMPILER_HPP

#include "ast.hpp"
#include "vm.hpp"

namespace eml {

/**
 * @brief Compiles the AST Expr node expr into bytecode
 */
auto bytecode_from_ast(const ast::Expr& expr) -> chunk;

/**
 * @brief Compiles source code string into byte-code chunk
 */
std::optional<eml::chunk> compile(std::string_view source);

} // namespace eml

#endif // EML_COMPILER_HPP
