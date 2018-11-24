#include "value.hpp"

#include <sstream>

namespace eml {

auto to_string(const eml::Value& v, eml::PrintType print_type) -> std::string
{
  std::stringstream ss;
  switch (v.type) {
  case Value::type::Number:
    ss << v.val.num;
    if (print_type == PrintType::yes) {
      ss << ": Number";
    }
    break;
  case Value::type::Boolean:
    ss << std::boolalpha << v.val.boolean;
    if (print_type == PrintType::yes) {
      ss << ": Bool";
    }
    break;
  case Value::type::Unit:
    ss << "()";
    if (print_type == PrintType::yes) {
      ss << ": Unit";
    }
    break;
  }

  return ss.str();
}

auto operator<<(std::ostream& s, const Value& v) -> std::ostream&
{
  s << to_string(v);
  return s;
}

} // namespace eml
