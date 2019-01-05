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

[[nodiscard]] auto make_string(std::string_view s, GarbageCollector& gc)
    -> GcPointer;

/**
 * @brief Concatenates to string object into on string
 */
[[nodiscard]] auto string_cat(GcPointer a, GcPointer b, GarbageCollector& gc)
    -> GcPointer;

} // namespace eml

#endif // EML_STRING_HPP
