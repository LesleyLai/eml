#include "expected.hpp"
#include <catch2/catch.hpp>

TEST_CASE("Simple assignment", "[assignment.simple]")
{
  eml::expected<int, int> e1 = 42;
  eml::expected<int, int> e2 = 17;
  eml::expected<int, int> e3 = 21;
  eml::expected<int, int> e4 = eml::make_unexpected(42);
  eml::expected<int, int> e5 = eml::make_unexpected(17);
  eml::expected<int, int> e6 = eml::make_unexpected(21);

  e1 = e2;
  REQUIRE(e1);
  REQUIRE(*e1 == 17);
  REQUIRE(e2);
  REQUIRE(*e2 == 17);

  e1 = std::move(e2);
  REQUIRE(e1);
  REQUIRE(*e1 == 17);
  REQUIRE(e2);
  REQUIRE(*e2 == 17);

  e1 = 42;
  REQUIRE(e1);
  REQUIRE(*e1 == 42);

  auto unex = eml::make_unexpected(12);
  e1 = unex;
  REQUIRE(!e1);
  REQUIRE(e1.error() == 12);

  e1 = eml::make_unexpected(42);
  REQUIRE(!e1);
  REQUIRE(e1.error() == 42);

  e1 = e3;
  REQUIRE(e1);
  REQUIRE(*e1 == 21);

  e4 = e5;
  REQUIRE(!e4);
  REQUIRE(e4.error() == 17);

  e4 = std::move(e6);
  REQUIRE(!e4);
  REQUIRE(e4.error() == 21);

  e4 = e1;
  REQUIRE(e4);
  REQUIRE(*e4 == 21);
}

TEST_CASE("Assignment deletion", "[assignment.deletion]")
{
  struct has_all {
    has_all() = default;
    has_all(const has_all&) = default;
    has_all(has_all&&) noexcept = default;
    has_all& operator=(const has_all&) = default;
  };

  eml::expected<has_all, has_all> e1 = {};
  eml::expected<has_all, has_all> e2 = {};
  e1 = e2;
}
