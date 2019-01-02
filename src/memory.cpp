#include "memory.hpp"

namespace eml {

GarbageCollector::~GarbageCollector()
{
  Obj* object = root_;
  while (object != nullptr) {
    Obj* next = object->next();

    const std::size_t allocate_size = sizeof(Obj) - 1 + object->size();

    object->~Obj();
    std::free(object);

    object = next;
  }
}

GarbageCollector::GarbageCollector(GarbageCollector&& other) noexcept
    : root_{other.root_}
{
  other.root_ = nullptr;
}

auto GarbageCollector::operator=(GarbageCollector&& other) noexcept
    -> GarbageCollector&
{
  std::swap(root_, other.root_);
  return *this;
}

auto GarbageCollector::allocate(std::size_t bytes) -> GcPointer
{
  const std::size_t allocate_size = sizeof(Obj) - 1 + bytes;
  void* ptr = std::malloc(allocate_size);
  auto* object = new (ptr) Obj{bytes, root_};
  root_ = object;
  return GcPointer{object};
}

} // namespace eml
