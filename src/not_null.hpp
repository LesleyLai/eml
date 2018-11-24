#ifndef EML_NOT_NULL_HPP
#define EML_NOT_NULL_HPP

/**
 * @file not_null.hpp
 * @brief contains the eml::not_null type
 */

#include <cstddef>
#include <iostream>
#include <type_traits>

#include "common.hpp"

namespace eml {

/**
 * @brief Restricts a pointer or smart pointer to only hold non-null values.
 *
 * If T is a pointer (i.e. T == U*) then
 * - allow construction from U*
 * - disallow construction from nullptr_t
 * - disallow default construction
 * - Assert construction from null U* fails, but not enforce if assertion is
 * disabled
 * - allow implicit conversion to U*
 */
template <class T> class not_null {
public:
  static_assert(std::is_assignable<T&, std::nullptr_t>::value,
                "T cannot be assigned nullptr.");

  template <typename U,
            typename = std::enable_if_t<std::is_convertible<U, T>::value>>
  constexpr explicit not_null(U&& u) : ptr_(std::forward<U>(u))
  {
    assert(ptr_ != nullptr);
  }

  template <
      typename = std::enable_if_t<!std::is_same<std::nullptr_t, T>::value>>
  constexpr explicit not_null(T u) : ptr_(u)
  {
    assert(ptr_ != nullptr);
  }

  template <typename U,
            typename = std::enable_if_t<std::is_convertible<U, T>::value>>
  constexpr not_null(const not_null<U>& other) : not_null(other.get()) // NOLINT
  {
  }

  not_null(not_null&& other) noexcept(std::is_nothrow_move_constructible_v<T>) =
      default;
  not_null(const not_null& other) = default;
  not_null& operator=(not_null&& other) noexcept(
      std::is_nothrow_move_assignable_v<T>) = default;
  not_null& operator=(const not_null& other) = default;
  ~not_null() = default;

  constexpr T get() const
  {
    Ensures(ptr_ != nullptr);
    return ptr_;
  }

  constexpr operator T() const // NOLINT
  {
    return get();
  }
  constexpr T operator->() const
  {
    return get();
  }
  constexpr decltype(auto) operator*() const
  {
    return *get();
  }

  // prevents compilation when someone attempts to assign a null pointer
  // constant
  not_null(std::nullptr_t) = delete;
  not_null& operator=(std::nullptr_t) = delete;

  // unwanted operators...pointers only point to single objects!
  not_null& operator++() = delete;
  not_null& operator--() = delete;
  const not_null operator++(int) = delete;
  const not_null operator--(int) = delete;
  not_null& operator+=(std::ptrdiff_t) = delete;
  not_null& operator-=(std::ptrdiff_t) = delete;
  void operator[](std::ptrdiff_t) const = delete;

private:
  T ptr_;
};

template <class T> auto make_not_null(T&& t)
{
  return eml::not_null<std::remove_cv_t<std::remove_reference_t<T>>>{
      std::forward<T>(t)};
}

template <class T>
std::ostream& operator<<(std::ostream& os, const not_null<T>& val)
{
  os << val.get();
  return os;
}

template <class T, class U>
auto operator==(const not_null<T>& lhs, const not_null<U>& rhs)
    -> decltype(lhs.get() == rhs.get())
{
  return lhs.get() == rhs.get();
}

template <class T, class U>
auto operator!=(const not_null<T>& lhs, const not_null<U>& rhs)
    -> decltype(lhs.get() != rhs.get())
{
  return lhs.get() != rhs.get();
}

template <class T, class U>
auto operator<(const not_null<T>& lhs, const not_null<U>& rhs)
    -> decltype(lhs.get() < rhs.get())
{
  return lhs.get() < rhs.get();
}

template <class T, class U>
auto operator<=(const not_null<T>& lhs, const not_null<U>& rhs)
    -> decltype(lhs.get() <= rhs.get())
{
  return lhs.get() <= rhs.get();
}

template <class T, class U>
auto operator>(const not_null<T>& lhs, const not_null<U>& rhs)
    -> decltype(lhs.get() > rhs.get())
{
  return lhs.get() > rhs.get();
}

template <class T, class U>
auto operator>=(const not_null<T>& lhs, const not_null<U>& rhs)
    -> decltype(lhs.get() >= rhs.get())
{
  return lhs.get() >= rhs.get();
}

// more unwanted operators
template <class T, class U>
std::ptrdiff_t operator-(const not_null<T>&, const not_null<U>&) = delete;
template <class T>
not_null<T> operator-(const not_null<T>&, std::ptrdiff_t) = delete;
template <class T>
not_null<T> operator+(const not_null<T>&, std::ptrdiff_t) = delete;
template <class T>
not_null<T> operator+(std::ptrdiff_t, const not_null<T>&) = delete;

} // namespace eml

namespace std {
template <class T> struct hash<eml::not_null<T>> {
  std::size_t operator()(const eml::not_null<T>& value) const
  {
    return hash<T>{}(value);
  }
};

} // namespace std

#endif // EML_NOT_NULL_HPP
