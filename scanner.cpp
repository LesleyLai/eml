#include "scanner.hpp"

namespace eml {

std::ostream& operator<<(std::ostream& s, token t)
{
  s << '(' << static_cast<int>(t.type) << ": \"" << t.text << "\" L" << t.line
    << ':' << t.column << ')';
  return s;
}

} // namespace eml
