#ifndef EML_OBJECT_HPP
#define EML_OBJECT_HPP

#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>

#include "common.hpp"

namespace eml {

/**
 * @brief Print type with the value
 */
enum PrintType {
  yes,
  no,
};

/**
 * @brief Base class for all the heap allocated object in EML
 */
class Obj {
public:
  virtual auto to_string(PrintType print_type) -> std::string = 0;

  Obj() = default;
  virtual ~Obj() = default;
  Obj(const Obj&) = default;
  Obj& operator=(const Obj&) = default;
  Obj(Obj&&) = default;
  Obj& operator=(Obj&&) = default;
};

/**
 * @brief String objects in EML
 */
class StringObj : public Obj {
public:
  explicit StringObj(std::string_view s) : size_{s.size()}
  {
    EML_ASSERT(s.size() <= std::numeric_limits<std::uint32_t>::max(),
               "Cannot handle a string that long");

    data_ = static_cast<const char*>(std::malloc(size_));
    std::uninitialized_copy(std::begin(s), std::end(s), data_);
  }

  auto to_string(PrintType print_type) -> std::string override
  {
    auto text = std::string{data_, size_};
    if (print_type == PrintType::yes) {
      text += ": String";
    }
    return text;
  }

private:
  std::uint32_t size_; // Does not care about strings greater than this
  const char* data_;
};

} // namespace eml

#endif // EML_OBJECT_HPP
