#include "eml.hpp"
#include "compiler.hpp"
#include "parser.hpp"

namespace eml {

CompileResult compile(std::string_view src)
{
  return eml::parse(src).map(
      [](const auto& ast) { return eml::bytecode_from_ast(*ast); });
}

} // namespace eml
