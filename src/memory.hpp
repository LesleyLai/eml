#ifndef EML_MEMORY_HPP
#define EML_MEMORY_HPP

#include <functional>
#include <memory_resource>

namespace eml {

/**
 * @brief Print type with the value
 */
enum PrintType {
  yes,
  no,
};

class GarbageCollector;

/**
 * @brief A heap allocated, and garbage-collection managed object in EML
 */
class Obj {
public:
  [[nodiscard]] constexpr auto size() const noexcept -> std::size_t
  {
    return size_;
  }

  [[nodiscard]] constexpr auto next() noexcept -> Obj*
  {
    return next_;
  }

  [[nodiscard]] constexpr auto data() const noexcept -> const std::byte*
  {
    return static_cast<const std::byte*>(data_);
  }

  [[nodiscard]] constexpr auto data() noexcept -> std::byte*
  {
    return static_cast<std::byte*>(data_);
  }

private:
  std::size_t size_; // Does not care about strings greater than this
  Obj* next_ = nullptr;
  std::byte data_[1];

  constexpr explicit Obj(std::size_t size, Obj* next)
      : size_{size}, next_{next}, data_{}
  {
  }

  friend GarbageCollector;
};

class GarbageCollector {
public:
  explicit GarbageCollector(std::pmr::memory_resource& underlying)
      : underlying_{underlying}
  {
  }

  ~GarbageCollector()
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

  auto allocate(std::size_t bytes) -> Obj*
  {
    const std::size_t allocate_size = sizeof(Obj) - 1 + bytes;
    void* ptr = underlying_.get().allocate(allocate_size);
    auto* object = new (ptr) Obj{bytes, root_};
    root_ = object;
    return object;
  }

  auto is_equal(const GarbageCollector& other) const noexcept -> bool
  {
    return this == &other;
  }

private:
  std::reference_wrapper<std::pmr::memory_resource> underlying_;
  Obj* root_ = nullptr; // List of allocated objects
};

} // namespace eml

#endif // EML_MEMORY_HPP
