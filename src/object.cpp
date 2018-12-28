#include "object.hpp"

namespace eml {

StringObj::StringObj(std::string_view s)
    : size_{static_cast<std::uint32_t>(s.size())}
{
  EML_ASSERT(s.size() <= std::numeric_limits<std::uint32_t>::max(),
             "Cannot handle a string that long");

  data_ = static_cast<const char*>(std::malloc(size_));
  std::uninitialized_copy(std::begin(s), std::end(s), data_);
}

auto StringObj::to_string(eml::PrintType print_type) -> std::string
{
  auto text = std::string{data_, size_};
  if (print_type == PrintType::yes) {
    text += ": String";
  }
  return text;
}

} // namespace eml
