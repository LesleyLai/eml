#ifndef EML_COMMON_HPP
#define EML_COMMON_HPP

#include <string_view>

/**
 * @file This header contains macros and defines used across the entire EML
 * implementation. In particular, it contains compile time configurations
 * defines that control how EML works.
 */

namespace eml {

struct BuildOptions {
#ifdef EML_VM_DEBUG_TRACE_EXECUTION
  constexpr static bool vm_debug_trace_execution = true;
#else
  constexpr static bool vm_debug_trace_execution = false;
#endif
};
inline constexpr BuildOptions build_options;

struct version {
  static constexpr int major = EML_MAJOR_VERSION;
  static constexpr int minor = EML_MINOR_VERSION;
  static constexpr int patch = EML_PATCH_VERSION;
  static constexpr std::string_view to_string()
  {
    return EML_VERSION;
  }
};

} // namespace eml

#ifdef DEBUG
// Indicates that we know execution should never reach this point in the
// program. In debug mode, we assert this fact because it's a bug to get here.
//
// In release mode, we use compiler-specific built in functions to tell the
// compiler the code can't be reached. This avoids "missing return" warnings
// in some cases and also lets it perform some optimizations by assuming the
// code is never reached.
#define EML_UNREACHABLE()                                                      \
  do {                                                                         \
    fprintf(stderr, "[%s:%d] This code should not be reached in %s()\n",       \
            __FILE__, __LINE__, __func__);                                     \
    abort();                                                                   \
  } while (0)
#else
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
