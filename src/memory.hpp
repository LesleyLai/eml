#ifndef EML_MEMORY_HPP
#define EML_MEMORY_HPP

#include <cstring>

#include "common.hpp"

namespace eml {

/// bit_cast<Dest,Source> is a template function that implements the
/// equivalent of "*reinterpret_cast<Dest*>(&source)".
template <class Dest, class Source> inline Dest bit_cast(Source const& source)
{
  static_assert(sizeof(Dest) == sizeof(Source),
                "size of destination and source objects must be equal");
  static_assert(std::is_trivially_copyable<Dest>::value,
                "destination type must be trivially copyable.");
  static_assert(std::is_trivially_copyable<Source>::value,
                "source type must be trivially copyable");

  Dest dest;
  std::memcpy(&dest, &source, sizeof(dest));
  return dest;
}

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
class GcPointer {
public:
  constexpr explicit GcPointer(Obj* obj) noexcept : obj_{obj}
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

  [[nodiscard]] constexpr auto operator==(const GcPointer& other) noexcept
      -> bool
  {
    return obj_ == other.obj_;
  }

private:
  Obj* obj_;
};

class GarbageCollector {
public:
  explicit GarbageCollector() = default;

  ~GarbageCollector();

  GarbageCollector(const GarbageCollector& other) = delete;
  auto operator=(const GarbageCollector& other) -> GarbageCollector& = delete;
  GarbageCollector(GarbageCollector&& other) noexcept;
  auto operator=(GarbageCollector&& other) noexcept -> GarbageCollector&;

  auto allocate(std::size_t bytes) -> GcPointer;

  auto is_equal(const GarbageCollector& other) const noexcept -> bool
  {
    return this == &other;
  }

private:
  Obj* root_ = nullptr; // List of allocated objects
};

} // namespace eml

#endif // EML_MEMORY_HPP
