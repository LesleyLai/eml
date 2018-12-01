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
      expect_ast = eml::type_check(*expect_ast);
    }
    return expect_ast.map(
        [this](const auto& ast) { return this->bytecode_from_ast(*ast); });
  }

  /**
   * @brief Compiles the AST Expr node expr into bytecode
   */
  auto bytecode_from_ast(const eml::ast::AstNode& expr) -> Bytecode;

  /**
   * @brief  Adds a global constants to a byte code chunk
   */
  void add_global(std::string_view identifier, Type t, Value v)
  {
    if (globals_.count(std::string{identifier}) > 0) { // Shadowing

      if (options_.shadowing_policy == Shadowing::warning_on_same_scope) {
        std::clog << "Warning: Global value definition of " << identifier
                  << " shadows earlier binding "
                     "in the global scope\n";
      }
    }

    globals_.emplace(identifier, std::pair{std::move(t), std::move(v)});
  }

private:
  CompilerConfig options_;
  std::unordered_map<std::string, const std::pair<Type, Value>>
      globals_; // Identifier to (type, value index) mapping for globals
};

} // namespace eml

#endif // EML_COMPILER_HPP
