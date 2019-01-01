#include "string.hpp"

namespace eml {

auto make_string(std::string_view s, GarbageCollector& gc) -> Ref
{
  Ref result = gc.allocate(s.size());
  std::uninitialized_copy(s.begin(), s.end(),
                          bit_cast<unsigned char*>(result->data()));
  return result;
}

} // namespace eml
