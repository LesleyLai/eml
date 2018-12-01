#ifndef EML_COMPILER_HPP
#define EML_COMPILER_HPP

#include <string_view>
#include <unordered_map>
#include <vector>

#include "bytecode.hpp"
#include "error.hpp"
#include "expected.hpp"
#include "type.hpp"
#include "value.hpp"

#include "parser.hpp"
#include "type_checker.hpp"

/**
 * @file compiler.hpp
 * @brief This file contains the main compiler class of EML
 */

namespace eml {

/**
 * @brief The compiler policy about shadowing
 *
 * This runtime policy enum decides whether the compiler will silently allow
 * name shadowing, or provides a warning message
 */
enum class Shadowing {
  allow,                 ///< @brief Allow shadowing without any warning
  warning_on_same_scope, ///< @brief Warn on shadowing in the same scope
};

struct CompilerConfig {
  Shadowing shadowing_policy = Shadowing::warning_on_same_scope;
};

/**
 * @brief The compiler for the EML
 *
 * This class provides the API for the EML frontend.
 */
class Compiler {
public:
  using TypeCheckResult =
      expected<std::unique_ptr<ast::AstNode>, std::vector<CompilationError>>;
  using CompileResult = expected<Bytecode, std::vector<CompilationError>>;

  /**
   * @brief
   * @arg options Runtime configuration of the compiler. If unprovided, have
   * sensible defaults
   */
  explicit Compiler(CompilerConfig options = {}) : options_{options} {}

  /**
   * @brief compiles the source into bytecode
   *
   * @return A bytecode chunk if the compilation process succeed, a vector of
   * errors otherwise
   */
  auto compile(std::string_view src) -> CompileResult
  {
    auto expect_ast = eml::parse(src);
    if (expect_ast) {
      expect_ast = this->type_check(*expect_ast);
    }
    return expect_ast.map(
        [this](const auto& ast) { return this->bytecode_from_ast(*ast); });
  }

  /**
   * @brief Compiles the AST Expr node expr into bytecode
   */
  auto bytecode_from_ast(const eml::ast::AstNode& expr) const -> Bytecode;

  /**
   * @brief  Adds a global constants to a byte code chunk
   */
  void add_global(const std::string& identifier, Type t, Value v)
  {
    auto query_result = global_env_.find(identifier);
    if (query_result != global_env_.end()) { // Shadowing

      if (options_.shadowing_policy == Shadowing::warning_on_same_scope) {
        std::clog << "Warning: Global value definition of " << identifier
                  << " shadows earlier binding "
                     "in the global scope\n";
      }

      global_env_[identifier] = std::pair{std::move(t), std::move(v)};
    } else {
      global_env_.emplace(identifier, std::pair{std::move(t), std::move(v)});
    }
  }

  /**
   * @brief Gets the global value from the envirnment if it exist
   */
  [[nodiscard]] auto get_global(std::string_view identifier) const
      -> std::optional<const std::pair<Type, Value>>
  {
    const auto pos = global_env_.find(std::string{identifier});
    if (pos != global_env_.end()) {
      return {pos->second};
    } else {
      return {};
    }
  }

  /**
   * @brief Check the type of the ast
   *
   * This function returns an ast that all nodes have types if successful, or a
   * vector of error if it find type errors
   */
  auto type_check(std::unique_ptr<ast::AstNode>& ptr) -> TypeCheckResult;

private:
  CompilerConfig options_;
  std::unordered_map<std::string, std::pair<Type, Value>>
      global_env_; // Identifier to (type, value index) mapping for globals
};

} // namespace eml

#endif // EML_COMPILER_HPP
