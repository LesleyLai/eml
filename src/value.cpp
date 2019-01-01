#include "value.hpp"

#include <sstream>

namespace eml {

struct TypeValuePrinter {
  const Value& v;
  PrintType print_type;

  auto operator()(const NumberType&) -> std::string
  {
    std::stringstream ss;
    ss << v.unsafe_as_number();
    if (print_type == PrintType::yes) {
      ss << ": Number";
    }
    return ss.str();
  }

  auto operator()(const BoolType&) -> std::string
  {
    std::stringstream ss;
    ss << std::boolalpha << v.unsafe_as_boolean();
    if (print_type == PrintType::yes) {
      ss << ": Bool";
    }
    return ss.str();
  }

  auto operator()(const UnitType&) -> std::string
  {
    std::stringstream ss;
    ss << "()";
    if (print_type == PrintType::yes) {
      ss << ": Unit";
    }
    return ss.str();
  }

  [[noreturn]] auto operator()(const ErrorType&) -> std::string
  {
    EML_UNREACHABLE();
  }

  auto operator()(const StringType&) -> std::string
  {
    const auto ref = v.unsafe_as_reference();
    std::string s{reinterpret_cast<const char*>(ref->data()), ref->size()};
    s = "\"" + s + "\"";
    if (print_type == PrintType::yes) {
      s += ": String";
    }
    return s;
  }
};

auto to_string(const Type& t, const Value& v, PrintType print_type)
    -> std::string
{
  TypeValuePrinter printer{v, print_type};
  return std::visit(printer, t);
}

} // namespace eml
