#include "memory.hpp"

namespace eml {

GarbageCollector::~GarbageCollector()
{
  Obj* object = root_;
  while (object != nullptr) {
    Obj* next = object->next();

    const std::size_t allocate_size = sizeof(Obj) - 1 + object->size();

    object->~Obj();
    underlying_.get().deallocate(object, allocate_size);

    object = next;
  }
}

GarbageCollector::GarbageCollector(GarbageCollector&& other) noexcept
    : underlying_{other.underlying_}, root_{other.root_}
{
  other.root_ = nullptr;
}

auto GarbageCollector::operator=(GarbageCollector&& other) noexcept
    -> GarbageCollector&
{
  std::swap(underlying_, other.underlying_);
  std::swap(root_, other.root_);
  return *this;
}

auto GarbageCollector::allocate(std::size_t bytes) -> Ref
{
  const std::size_t allocate_size = sizeof(Obj) - 1 + bytes;
  void* ptr = underlying_.get().allocate(allocate_size);
  auto* object = new (ptr) Obj{bytes, root_};
  root_ = object;
  return Ref{object};
}

} // namespace eml
