#include "expected.hpp"
#include <catch2/catch.hpp>

struct move_detector {
  move_detector() = default;
  move_detector(move_detector&& rhs)
  {
    rhs.been_moved = true;
  }
  bool been_moved = false;
};

TEST_CASE("Observers", "[observers]")
{
  eml::expected<int, int> o1 = 42;
  eml::expected<int, int> o2{eml::unexpect, 0};
  const eml::expected<int, int> o3 = 42;

  REQUIRE(*o1 == 42);
  REQUIRE(*o1 == o1.value());
  REQUIRE(o2.value_or(42) == 42);
  REQUIRE(o2.error() == 0);
  REQUIRE(o3.value() == 42);
  auto success = std::is_same<decltype(o1.value()), int&>::value;
  REQUIRE(success);
  success = std::is_same<decltype(o3.value()), const int&>::value;
  REQUIRE(success);
  success = std::is_same<decltype(std::move(o1).value()), int&&>::value;
  REQUIRE(success);

#ifndef TL_EXPECTED_NO_CONSTRR
  success = std::is_same<decltype(std::move(o3).value()), const int&&>::value;
  REQUIRE(success);
#endif

  eml::expected<move_detector, int> o4{eml::in_place};
  move_detector o5 = std::move(o4).value();
  REQUIRE(o4->been_moved);
  REQUIRE(!o5.been_moved);
}
