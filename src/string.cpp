#include "string.hpp"

namespace eml {

auto make_string(std::string_view s, GarbageCollector& gc) -> Obj*
{
  Obj* obj = static_cast<Obj*>(gc.allocate(s.size()));
  std::uninitialized_copy(s.begin(), s.end(),
                          reinterpret_cast<unsigned char*>(obj->data()));
  return obj;
}

} // namespace eml
