#include "value.hpp"

struct value_printer {
  std::ostream& s;

  void operator()(double d) { s << d; }
};

auto eml::operator<<(std::ostream& s, const eml::value& v) -> std::ostream&
{
  std::visit(value_printer{s}, v);
  return s;
}
