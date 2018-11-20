#include "scanner.hpp"

namespace eml {

std::ostream& operator<<(std::ostream& s, token_type t)
{
  switch (t) {
#define TOKEN_TABLE_ENTRY(type, type_name, prefix, infix, precedence)          \
  case token_type::type:                                                       \
    s << std::string_view(type_name);                                          \
    break;

#include "token_table.inc"
#undef TOKEN_TABLE_ENTRY
  }
  return s;
}

std::ostream& operator<<(std::ostream& s, token t)
{
  s << "(token: " << t.type << "\n text: \"" << t.text << "\"\n L" << t.line
    << ':' << t.column << ")\n";
  return s;
}

} // namespace eml
