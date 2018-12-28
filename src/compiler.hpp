#ifndef EML_COMPILER_HPP
#define EML_COMPILER_HPP

#include <string_view>
#include <unordered_map>
#include <vector>

#include "bytecode.hpp"
#include "error.hpp"
#include "expected.hpp"
#include "module.hpp"
#include "type.hpp"
#include "value.hpp"

#include "parser.hpp"

/**
 * @file compiler.hpp
 * @brief This file contains the main compiler class of EML
 */

namespace eml {

/**
 * @brief Runtime configurations that decides how the eml compiler should behave
 */
struct CompilerConfig {
  SameScopeShadowing shadowing_policy = SameScopeShadowing::warning;
};

/**
 * @brief The compiler for the EML
 * This class provides the API for the EML frontend.
 */
class Compiler {
public:
  using TypeCheckResult =
      expected<std::unique_ptr<AstNode>, std::vector<CompilationError>>;
  using CompileResult = expected<Bytecode, std::vector<CompilationError>>;

  /**
   * @brief Constructs a compiler object
   * @arg options Runtime configuration of the compiler. If unprovided, have
   * sensible defaults
   */
  explicit Compiler(CompilerConfig options = {}) noexcept : options_{options} {}

  /**
   * @brief compiles the source into bytecode
   *
   * @return A bytecode chunk if the compilation process succeed, a vector of
   * errors otherwise
   */
  auto compile(std::string_view src) -> CompileResult
  {
    return eml::parse(src)
        .and_then([this](auto ast) { return type_check(ast); })
        .map([this](const auto& ast) { return bytecode_from_ast(*ast); });
  }

  /**
   * @brief Compiles the AST Expr node expr into bytecode
   */
  auto bytecode_from_ast(const eml::AstNode& expr) const -> Bytecode;

  /**
   * @brief  Adds a global constants to a byte code chunk
   */
  void add_global(const std::string& identifier, Type t, Value v)
  {
    auto query_result = constexpr_env_.find(identifier);
    if (query_result != constexpr_env_.end()) { // Shadowing

      if (options_.shadowing_policy == SameScopeShadowing::warning) {
        std::clog << "Warning: Global value definition of " << identifier
                  << " shadows earlier binding "
                     "in the global scope\n";
      }

      constexpr_env_[identifier] = std::pair{std::move(t), std::move(v)};
    } else {
      constexpr_env_.emplace(identifier, std::pair{std::move(t), std::move(v)});
    }
  }

  /**
   * @brief Gets the global value from the envirnment if it exist
   */
  [[nodiscard]] auto get_global(std::string_view identifier) const
      -> std::optional<const std::pair<Type, Value>>
  {
    const auto pos = constexpr_env_.find(std::string{identifier});
    if (pos != constexpr_env_.end()) {
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
  auto type_check(std::unique_ptr<AstNode>& ptr) -> TypeCheckResult;

private:
  CompilerConfig options_;
  std::unordered_map<std::string, std::pair<Type, Value>>
      constexpr_env_; // Identifier to (type, value index) mapping for globals
};

} // namespace eml

#endif // EML_COMPILER_HPP
