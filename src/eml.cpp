#include "eml.hpp"
#include "compiler.hpp"
#include "parser.hpp"
#include "type_checker.hpp"

namespace eml {

CompileResult compile(std::string_view src)
{
  auto expect_ast = eml::parse(src);
  if (expect_ast) {
    expect_ast = eml::type_check(*expect_ast);
  }
  return expect_ast.map(
      [](const auto& ast) { return eml::bytecode_from_ast(*ast); });
}

} // namespace eml
