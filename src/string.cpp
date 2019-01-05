#include "string.hpp"

namespace eml {

auto make_string(std::string_view s, GarbageCollector& gc) -> GcPointer
{
  GcPointer result = gc.allocate(s.size());
  std::uninitialized_copy(s.begin(), s.end(),
                          bit_cast<unsigned char*>(result->data()));
  return result;
}

auto string_cat(GcPointer a, GcPointer b, GarbageCollector& gc) -> GcPointer
{
  GcPointer result = gc.allocate(a->size() + b->size());
  std::uninitialized_copy(a->data(), a->data() + a->size(), result->data());
  std::uninitialized_copy(b->data(), b->data() + b->size(),
                          result->data() + a->size());
  return result;
}

} // namespace eml
