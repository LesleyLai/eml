#include "expected.hpp"
#include <catch2/catch.hpp>
#include <memory>

TEST_CASE("Map extensions", "[expected.map]")
{
  auto mul2 = [](int a) { return a * 2; };
  auto ret_void = [](int) {};

  {
    eml::expected<int, int> e = 21;
    auto ret = e.map(mul2);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    const eml::expected<int, int> e = 21;
    auto ret = e.map(mul2);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    eml::expected<int, int> e = 21;
    auto ret = e.map(mul2);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    const eml::expected<int, int> e = 21;
    auto ret = e.map(mul2);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.map(mul2);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    const eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.map(mul2);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.map(mul2);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    const eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.map(mul2);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    eml::expected<int, int> e = 21;
    auto ret = e.map(ret_void);
    REQUIRE(ret);
    STATIC_REQUIRE(
        (std::is_same<decltype(ret), eml::expected<void, int>>::value));
  }

  {
    const eml::expected<int, int> e = 21;
    auto ret = e.map(ret_void);
    REQUIRE(ret);
    STATIC_REQUIRE(
        (std::is_same<decltype(ret), eml::expected<void, int>>::value));
  }

  {
    eml::expected<int, int> e = 21;
    auto ret = e.map(ret_void);
    REQUIRE(ret);
    STATIC_REQUIRE(
        (std::is_same<decltype(ret), eml::expected<void, int>>::value));
  }

  {
    const eml::expected<int, int> e = 21;
    auto ret = e.map(ret_void);
    REQUIRE(ret);
    STATIC_REQUIRE(
        (std::is_same<decltype(ret), eml::expected<void, int>>::value));
  }

  {
    eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.map(ret_void);
    REQUIRE(!ret);
    STATIC_REQUIRE(
        (std::is_same<decltype(ret), eml::expected<void, int>>::value));
  }

  {
    const eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.map(ret_void);
    REQUIRE(!ret);
    STATIC_REQUIRE(
        (std::is_same<decltype(ret), eml::expected<void, int>>::value));
  }

  {
    eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.map(ret_void);
    REQUIRE(!ret);
    STATIC_REQUIRE(
        (std::is_same<decltype(ret), eml::expected<void, int>>::value));
  }

  {
    const eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.map(ret_void);
    REQUIRE(!ret);
    STATIC_REQUIRE(
        (std::is_same<decltype(ret), eml::expected<void, int>>::value));
  }

  // mapping functions which return references
  {
    eml::expected<int, int> e(42);
    auto ret = e.map([](int& i) -> int& { return i; });
    REQUIRE(ret);
    REQUIRE(ret == 42);
  }
}

TEST_CASE("Map error extensions", "[expected.map_error]")
{
  auto mul2 = [](int a) { return a * 2; };
  auto ret_void = [](int) {};

  {
    eml::expected<int, int> e = 21;
    auto ret = e.map_error(mul2);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    const eml::expected<int, int> e = 21;
    auto ret = e.map_error(mul2);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    eml::expected<int, int> e = 21;
    auto ret = e.map_error(mul2);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    const eml::expected<int, int> e = 21;
    auto ret = e.map_error(mul2);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.map_error(mul2);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 42);
  }

  {
    const eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.map_error(mul2);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 42);
  }

  {
    eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.map_error(mul2);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 42);
  }

  {
    const eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.map_error(mul2);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 42);
  }

  {
    eml::expected<int, int> e = 21;
    auto ret = e.map_error(ret_void);
    REQUIRE(ret);
  }

  {
    const eml::expected<int, int> e = 21;
    auto ret = e.map_error(ret_void);
    REQUIRE(ret);
  }

  {
    eml::expected<int, int> e = 21;
    auto ret = e.map_error(ret_void);
    REQUIRE(ret);
  }

  {
    const eml::expected<int, int> e = 21;
    auto ret = e.map_error(ret_void);
    REQUIRE(ret);
  }

  {
    eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.map_error(ret_void);
    REQUIRE(!ret);
  }

  {
    const eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.map_error(ret_void);
    REQUIRE(!ret);
  }

  {
    eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.map_error(ret_void);
    REQUIRE(!ret);
  }

  {
    const eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.map_error(ret_void);
    REQUIRE(!ret);
  }
}

TEST_CASE("And then extensions", "[expected.and_then]")
{
  auto succeed = [](int) { return eml::expected<int, int>(21 * 2); };
  auto fail = [](int) { return eml::expected<int, int>(eml::unexpect, 17); };

  {
    eml::expected<int, int> e = 21;
    auto ret = e.and_then(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    const eml::expected<int, int> e = 21;
    auto ret = e.and_then(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    eml::expected<int, int> e = 21;
    auto ret = e.and_then(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    const eml::expected<int, int> e = 21;
    auto ret = e.and_then(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    eml::expected<int, int> e = 21;
    auto ret = e.and_then(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 17);
  }

  {
    const eml::expected<int, int> e = 21;
    auto ret = e.and_then(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 17);
  }

  {
    eml::expected<int, int> e = 21;
    auto ret = e.and_then(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 17);
  }

  {
    const eml::expected<int, int> e = 21;
    auto ret = e.and_then(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 17);
  }

  {
    eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.and_then(succeed);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    const eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.and_then(succeed);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.and_then(succeed);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    const eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.and_then(succeed);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.and_then(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    const eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.and_then(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.and_then(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    const eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.and_then(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }
}

TEST_CASE("or_else", "[expected.or_else]")
{
  using eptr = std::unique_ptr<int>;
  auto succeed = [](int) { return eml::expected<int, int>(21 * 2); };
  auto succeedptr = [](eptr) { return eml::expected<int, eptr>(21 * 2); };
  auto fail = [](int) { return eml::expected<int, int>(eml::unexpect, 17); };
  auto efail = [](eptr e) {
    *e = 17;
    return eml::expected<int, eptr>(eml::unexpect, std::move(e));
  };

  auto failvoid = [](int) {};
  auto failvoidptr = [](const eptr&) { /* don't consume */ };
  auto consumeptr = [](eptr) {};
  // NOLINTNEXTLINE(clang-analyzer-cplusplus.NewDeleteLeaks)
  auto make_u_int = [](int n) { return std::make_unique<int>(n); };

  {
    eml::expected<int, int> e = 21;
    auto ret = e.or_else(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    const eml::expected<int, int> e = 21;
    auto ret = e.or_else(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    eml::expected<int, int> e = 21;
    auto ret = e.or_else(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    eml::expected<int, eptr> e = 21;
    auto ret = std::move(e).or_else(succeedptr);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    const eml::expected<int, int> e = 21;
    auto ret = e.or_else(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    eml::expected<int, int> e = 21;
    auto ret = e.or_else(fail);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    const eml::expected<int, int> e = 21;
    auto ret = e.or_else(fail);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    eml::expected<int, int> e = 21;
    auto ret = e.or_else(fail);
    REQUIRE(ret);
    REQUIRE(ret == 21);
  }

  {
    eml::expected<int, eptr> e = 21;
    auto ret = std::move(e).or_else(efail);
    REQUIRE(ret);
    REQUIRE(ret == 21);
  }

  {
    const eml::expected<int, int> e = 21;
    auto ret = e.or_else(fail);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.or_else(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    const eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.or_else(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.or_else(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    eml::expected<int, eptr> e(eml::unexpect, make_u_int(21));
    auto ret = std::move(e).or_else(succeedptr);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    const eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.or_else(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.or_else(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 17);
  }

  {
    eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.or_else(failvoid);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    const eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.or_else(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 17);
  }

  {
    const eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.or_else(failvoid);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.or_else(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 17);
  }

  {
    eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.or_else(failvoid);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    eml::expected<int, eptr> e(eml::unexpect, make_u_int(21));
    auto ret = std::move(e).or_else(failvoidptr);
    REQUIRE(!ret);
    REQUIRE(*ret.error() == 21);
  }

  {
    eml::expected<int, eptr> e(eml::unexpect, make_u_int(21));
    auto ret = std::move(e).or_else(consumeptr);
    REQUIRE(!ret);
    REQUIRE(ret.error() == nullptr);
  }

  {
    const eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.or_else(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 17);
  }

  {
    const eml::expected<int, int> e(eml::unexpect, 21);
    auto ret = e.or_else(failvoid);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }
}
struct S {
  int x;
};

struct F {
  int x;
};

TEST_CASE("14", "[issue.14]")
{
  auto res = eml::expected<S, F>{eml::unexpect, F{}};

  res.map_error([](F) {

  });
}

TEST_CASE("32", "[issue.32]")
{
  int i = 0;
  eml::expected<void, int> a;
  a.map([&i] { i = 42; });
  REQUIRE(i == 42);

  auto x = a.map([] { return 42; });
  REQUIRE(*x == 42);
}
