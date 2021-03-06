#ifndef EML_COMMON_HPP
#define EML_COMMON_HPP

#ifdef EML_DEBUG
#include <iostream>
#endif

#include <string_view>
#include <type_traits>

/**
 * @file common.hpp
 * @brief Compile time configurations and utilitize macros and functions
 *
 * This header contains macros and defines used across the entire EML
 * implementation. In particular, it contains compile time configurations
 * defines that control how EML works.
 */

namespace eml {

struct BuildOptions {
#ifdef EML_DEBUG_VM_TRACE_EXECUTION
  constexpr static bool debug_vm_trace_execution = true;
#else
  constexpr static bool debug_vm_trace_execution = false;
#endif

#ifdef EML_DEBUG_PRINT_AST
  constexpr static bool debug_print_ast = true;
#else
  constexpr static bool debug_print_ast = false;
#endif
};
static constexpr BuildOptions build_options;

[[nodiscard]] auto version() noexcept -> std::string_view;
[[nodiscard]] auto major_version() noexcept -> int;
[[nodiscard]] auto minor_version() noexcept -> int;
[[nodiscard]] auto patch_version() noexcept -> int;

/**
 * @brief A position in the file contains line number and column number
 */
struct FilePos {
  std::size_t line = 1;
  std::size_t column = 1;
};

/// @brief Types of the tokens
enum class token_type {
#define TOKEN_TABLE_ENTRY(type, name, prefix, infix, precedence) type,
#include "token_table.inc"
#undef TOKEN_TABLE_ENTRY
};

/// @brief The token is the intermidiate data structure that the scanner and
/// parser communicate
struct Token {
  token_type type = token_type::eof;
  std::string_view text;
  FilePos position = {};
};

/**
 * @brief Statically cast a pointer of IN to a pointer of OUT
 * @tparam IN The input type
 * @tparam OUT The output type
 * @warning If IN is a base class of OUT, then the input value must have runtime
 * type of OUT, otherwise the result is undefined
 */
template <typename OUT, typename IN>
constexpr auto polymorphic_cast(IN value) -> OUT
{
#ifdef EML_DEBUG
  return dynamic_cast<OUT>(value);
#else
  return static_cast<OUT>(value);
#endif
}

} // namespace eml

#ifdef EML_DEBUG
#define EML_ASSERT(condition, message)                                         \
  do {                                                                         \
    if (!(condition)) {                                                        \
      []() {                                                                   \
        std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "                \
                  << "Assert failed in "                                       \
                  << std::string_view{static_cast<const char*>(__func__)}      \
                  << ": "                                                      \
                  << std::string_view{static_cast<const char*>(message)}       \
                  << '\n'                                                      \
                  << "This is probabaly an internal bug of the Embedded ML "   \
                     "Implementation, please fill a bug report.\n";            \
        std::abort();                                                          \
      }();                                                                     \
    }                                                                          \
  } while (0)

// Indicates that we know execution should never reach this point in the
// program. In debug mode, we assert this fact because it's a bug to get here.
//
// In release mode, we use compiler-specific built in functions to tell the
// compiler the code can't be reached. This avoids "missing return" warnings
// in some cases and also lets it perform some optimizations by assuming the
// code is never reached.
#define EML_UNREACHABLE()                                                      \
  do {                                                                         \
    std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "                    \
              << "This code should not be reached in "                         \
              << std::string_view{static_cast<const char*>(__func__)}          \
              << "\nThis is probabaly an internal bug of the Embedded ML "     \
                 "Implementation, please fill a bug report.\n";                \
    std::abort();                                                              \
  } while (0)
#else
#define EML_ASSERT(condition, message)                                         \
  do {                                                                         \
  } while (0)

// Tell the compiler that this part of the code will never be reached.
#if defined(_MSC_VER)
#define EML_UNREACHABLE() __assume(0)
#elif defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
#define EML_UNREACHABLE() __builtin_unreachable()
#else
#define EML_UNREACHABLE()
#endif
#endif

#endif // EML_COMMON_HPP
