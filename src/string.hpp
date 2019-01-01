#ifndef EML_STRING_HPP
#define EML_STRING_HPP

/**
 * @file string.hpp
 * @brief String object related operations in Embedded ML
 */

#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>

#include "common.hpp"
#include "memory.hpp"

namespace eml {

auto make_string(std::string_view s, GarbageCollector& gc) -> Ref;

} // namespace eml

#endif // EML_STRING_HPP
