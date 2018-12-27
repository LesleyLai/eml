#include "type.hpp"
#include "common.hpp"

namespace eml {

namespace {

struct TypePrinter {
  std::ostream& os_;

  void operator()(NumberType)
  {
    os_ << "Number";
  }
  void operator()(BoolType)
  {
    os_ << "Bool";
  }
  void operator()(UnitType)
  {
    os_ << "Unit";
  }

  void operator()(StringType)
  {
    os_ << "String";
  }

  void operator()(ErrorType)
  {
    os_ << "Error";
  }
};

} // anonymous namespace

std::ostream& operator<<(std::ostream& os, const Type& type)
{
  std::visit(TypePrinter{os}, type);
  return os;
}

} // namespace eml
