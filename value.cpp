#include <iostream>

#include "value.hpp"

namespace eml {

auto operator<<(std::ostream& s, const eml::value& v) -> std::ostream&
{
  s << v.val.num;
  return s;
}

value operator-(const eml::value& v) { return eml::value{-v.val.num}; }

auto operator+(const value& v1, const value& v2) -> value
{
  return eml::value{v1.val.num + v2.val.num};
}

auto operator-(const value& v1, const value& v2) -> value
{
  return eml::value{v1.val.num - v2.val.num};
}

auto operator*(const value& v1, const value& v2) -> value
{
  return eml::value{v1.val.num * v2.val.num};
}

auto operator/(const value& v1, const value& v2) -> value
{
  return eml::value{v1.val.num / v2.val.num};
}

} // namespace eml
