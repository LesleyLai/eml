#include "expected.hpp"
#include <catch2/catch.hpp>

#include <string>

using std::string;

eml::expected<int, string> getInt3(int val)
{
  return val;
}

eml::expected<int, string> getInt2(int val)
{
  return val;
}

eml::expected<int, string> getInt1()
{
  return getInt2(5).and_then(getInt3);
}

TEST_CASE("Issue 1", "[expected.issues.1]")
{
  getInt1();
}

eml::expected<int, int> operation1()
{
  return 42;
}

eml::expected<std::string, int> operation2(int const /*val*/)
{
  return "Bananas";
}

TEST_CASE("Issue 17", "[expected.issues.17]")
{
  auto const intermediate_result = operation1();

  intermediate_result.and_then(operation2);
}

struct a {
};
struct b : a {
};

auto doit() -> eml::expected<std::unique_ptr<b>, int>
{
  return eml::make_unexpected(0);
}

TEST_CASE("Issue 23", "[expected.issues.23]")
{
  eml::expected<std::unique_ptr<a>, int> msg = doit();
  REQUIRE(!msg.has_value());
}

TEST_CASE("Issue 26", "[expected.issues.26]")
{
  eml::expected<a, int> exp = eml::expected<b, int>(eml::unexpect, 0);
  REQUIRE(!exp.has_value());
}

eml::expected<int, std::string> error()
{
  return eml::make_unexpected(std::string("error1 "));
}
std::string maperror(const std::string& s)
{
  return s + "maperror ";
}

TEST_CASE("Issue 30", "[expected.issues.30]")
{
  error().map_error(maperror);
}

TEST_CASE("Issue 33", "[expected.issues.33]")
{
  eml::expected<void, int> res{eml::unexpect, 0};
  REQUIRE(!res);
  res = res.map_error([](int) { return 42; });
  REQUIRE(res.error() == 42);
}

eml::expected<void, std::string> voidWork()
{
  return {};
}
eml::expected<int, std::string> work2()
{
  return 42;
}
void errorhandling(const std::string&) {}

TEST_CASE("Issue 34", "[expected.issues.34]")
{
  eml::expected<int, std::string> result = voidWork().and_then(work2);
  result.map_error([&](std::string result) { errorhandling(result); });
}

struct non_copyable {
  non_copyable(non_copyable&&) = default;
  non_copyable(non_copyable const&) = delete;
  non_copyable() = default;
};

TEST_CASE("Issue 42", "[expected.issues.42]")
{
  eml::expected<non_copyable, int>{}.map([](non_copyable) {});
}

TEST_CASE("Issue 43", "[expected.issues.43]")
{
  auto result = eml::expected<void, std::string>{};
  result = eml::make_unexpected(std::string{"foo"});
}
