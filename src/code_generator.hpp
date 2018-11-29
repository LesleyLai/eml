#ifndef EML_COMPILER_HPP
#define EML_COMPILER_HPP

/**
 * @file code_generator.hpp
 * @brief This file defines the functionality to generate bytecode from
 * [AST](@ref eml::ast::AstNode)
 */

#include "ast.hpp"
#include "error.hpp"
#include "expected.hpp"
#include "vm.hpp"

namespace eml {

/**
 * @brief Compiles the AST Expr node expr into bytecode
 */
auto bytecode_from_ast(const ast::AstNode& expr) -> chunk;

} // namespace eml

#endif // EML_COMPILER_HPP
