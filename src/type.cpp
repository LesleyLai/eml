#include "type.hpp"

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
};

} // anonymous namespace

std::ostream& operator<<(std::ostream& os, const Type& type)
{
  std::visit(TypePrinter{os}, type);
  return os;
}

} // namespace eml
