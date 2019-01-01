#ifndef EML_MEMORY_HPP
#define EML_MEMORY_HPP

#include <functional>
#include <memory_resource>

#include "common.hpp"

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

/**
 * @brief Reference to a Heap allocated, garbage collector managed object
 * @note Cannot be null
 */
class Ref {
public:
  constexpr explicit Ref(Obj* obj) noexcept : obj_{obj}
  {
    EML_ASSERT(obj_ != nullptr, "The refered to object cannot be null");
  }

  [[nodiscard]] constexpr auto operator-> () const noexcept -> Obj*
  {
    return obj_;
  }

  [[nodiscard]] constexpr auto operator*() const noexcept -> Obj&
  {
    return *obj_;
  }

  [[nodiscard]] constexpr auto operator==(const Ref& other) noexcept -> bool
  {
    return obj_ == other.obj_;
  }

private:
  Obj* obj_;
};

class GarbageCollector {
public:
  explicit GarbageCollector(std::pmr::memory_resource& underlying)
      : underlying_{underlying}
  {
  }

  ~GarbageCollector();

  GarbageCollector(const GarbageCollector& other) = delete;
  auto operator=(const GarbageCollector& other) -> GarbageCollector& = delete;
  GarbageCollector(GarbageCollector&& other) noexcept;
  auto operator=(GarbageCollector&& other) noexcept -> GarbageCollector&;

  auto allocate(std::size_t bytes) -> Ref;

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
