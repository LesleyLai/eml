#ifndef EML_META_HPP
#define EML_META_HPP

#include <type_traits>

/**
 * @file meta.hpp
 * @brief This header includes some meta programming helpers
 */

namespace eml {

/**
 * @brief If T1 is the base class of T2, or vice versa
 * @related is_polymorphic_with_v
 */
template <class T1, class T2>
using is_polymorphic_with =
    std::disjunction<std::is_base_of<T1, T2>, std::is_base_of<T2, T1>>;

/**
 * @brief Helper variable template for is_polymorphic_with
 * @related is_polymorphic_with
 */
template <class T1, class T2>
constexpr bool is_polymorphic_with_v = is_polymorphic_with<T1, T2>::value;

}  // namespace eml


#endif // EML_META_HPP
