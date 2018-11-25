#ifndef EML_TYPE_CHECKER_HPP
#define EML_TYPE_CHECKER_HPP

#include <expected.hpp>

#include "ast.hpp"
#include "error.hpp"

namespace eml {

using TypeCheckResult = expected<ast::Expr_ptr, std::vector<CompilationError>>;

/**
 * @brief Check the type of the ast
 *
 * This function returns an ast that all nodes have types if successful, or a
 * vector of error if it find type errors
 */
auto type_check(ast::Expr_ptr& ptr) -> TypeCheckResult;

} // namespace eml

#endif // EML_TYPE_CHECKER_HPP
