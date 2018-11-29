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

namespace eml {

using SymbolTable =
    std::unordered_map<std::string, const std::pair<Type, std::size_t>>;

/**
 * @brief The compiler for the EML
 */
class Compiler {
public:
  using CompileResult = expected<chunk, std::vector<CompilationError>>;

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
   * @brief Adds a global constants and returns its index
   */
  std::size_t add_global(std::string_view identifier, Type t, Value v)
  {
    globals_.push_back(std::move(v));
    const auto index = globals_.size();
    symbols_.emplace(identifier, std::pair{t, index});
    return index;
  }

  /**
   * @brief Compiles the AST Expr node expr into bytecode
   */
  auto bytecode_from_ast(const eml::ast::AstNode& expr) -> chunk;

private:
  SymbolTable symbols_; // Identifier to (type, value index) mapping for global
  // values
  std::vector<Value> globals_; // Global constant
};

} // namespace eml

#endif // EML_COMPILER_HPP
