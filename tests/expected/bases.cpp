#include "expected.hpp"
#include <catch2/catch.hpp>

#include <string>

TEST_CASE("Triviality", "[bases.triviality]")
{
  REQUIRE(std::is_trivially_copy_constructible<eml::expected<int, int>>::value);
  REQUIRE(std::is_trivially_copy_assignable<eml::expected<int, int>>::value);
  REQUIRE(std::is_trivially_move_constructible<eml::expected<int, int>>::value);
  REQUIRE(std::is_trivially_move_assignable<eml::expected<int, int>>::value);
  REQUIRE(std::is_trivially_destructible<eml::expected<int, int>>::value);

  REQUIRE(
      std::is_trivially_copy_constructible<eml::expected<void, int>>::value);
  REQUIRE(
      std::is_trivially_move_constructible<eml::expected<void, int>>::value);
  REQUIRE(std::is_trivially_destructible<eml::expected<void, int>>::value);

  {
    struct T {
      T(const T&) = default;
      T(T&&) = default;
      T& operator=(const T&) = default;
      T& operator=(T&&) = default;
      ~T() = default;
    };
    REQUIRE(std::is_trivially_copy_constructible<eml::expected<T, int>>::value);
    REQUIRE(std::is_trivially_copy_assignable<eml::expected<T, int>>::value);
    REQUIRE(std::is_trivially_move_constructible<eml::expected<T, int>>::value);
    REQUIRE(std::is_trivially_move_assignable<eml::expected<T, int>>::value);
    REQUIRE(std::is_trivially_destructible<eml::expected<T, int>>::value);
  }

  {
    struct T {
      T(const T&) {}
      T(T&&) {}
      T& operator=(const T&)
      {
        return *this;
      }
      T& operator=(T&&)
      {
        return *this;
      }
      ~T() {}
    };
    REQUIRE(
        !std::is_trivially_copy_constructible<eml::expected<T, int>>::value);
    REQUIRE(!std::is_trivially_copy_assignable<eml::expected<T, int>>::value);
    REQUIRE(
        !std::is_trivially_move_constructible<eml::expected<T, int>>::value);
    REQUIRE(!std::is_trivially_move_assignable<eml::expected<T, int>>::value);
    REQUIRE(!std::is_trivially_destructible<eml::expected<T, int>>::value);
  }
}

TEST_CASE("Deletion", "[bases.deletion]")
{
  REQUIRE(std::is_copy_constructible<eml::expected<int, int>>::value);
  REQUIRE(std::is_copy_assignable<eml::expected<int, int>>::value);
  REQUIRE(std::is_move_constructible<eml::expected<int, int>>::value);
  REQUIRE(std::is_move_assignable<eml::expected<int, int>>::value);
  REQUIRE(std::is_destructible<eml::expected<int, int>>::value);

  {
    struct T {
      T() = default;
    };
    REQUIRE(std::is_default_constructible<eml::expected<T, int>>::value);
  }

  {
    struct T {
      T(int) {}
    };
    REQUIRE(!std::is_default_constructible<eml::expected<T, int>>::value);
  }

  {
    struct T {
      T(const T&) = default;
      T(T&&) = default;
      T& operator=(const T&) = default;
      T& operator=(T&&) = default;
      ~T() = default;
    };
    REQUIRE(std::is_copy_constructible<eml::expected<T, int>>::value);
    REQUIRE(std::is_copy_assignable<eml::expected<T, int>>::value);
    REQUIRE(std::is_move_constructible<eml::expected<T, int>>::value);
    REQUIRE(std::is_move_assignable<eml::expected<T, int>>::value);
    REQUIRE(std::is_destructible<eml::expected<T, int>>::value);
  }

  {
    struct T {
      T(const T&) = delete;
      T(T&&) = delete;
      T& operator=(const T&) = delete;
      T& operator=(T&&) = delete;
    };
    REQUIRE(!std::is_copy_constructible<eml::expected<T, int>>::value);
    REQUIRE(!std::is_copy_assignable<eml::expected<T, int>>::value);
    REQUIRE(!std::is_move_constructible<eml::expected<T, int>>::value);
    REQUIRE(!std::is_move_assignable<eml::expected<T, int>>::value);
  }

  {
    struct T {
      T(const T&) = delete;
      T(T&&) = default;
      T& operator=(const T&) = delete;
      T& operator=(T&&) = default;
    };
    REQUIRE(!std::is_copy_constructible<eml::expected<T, int>>::value);
    REQUIRE(!std::is_copy_assignable<eml::expected<T, int>>::value);
    REQUIRE(std::is_move_constructible<eml::expected<T, int>>::value);
    REQUIRE(std::is_move_assignable<eml::expected<T, int>>::value);
  }

  {
    struct T {
      T(const T&) = default;
      T(T&&) = delete;
      T& operator=(const T&) = default;
      T& operator=(T&&) = delete;
    };
    REQUIRE(std::is_copy_constructible<eml::expected<T, int>>::value);
    REQUIRE(std::is_copy_assignable<eml::expected<T, int>>::value);
  }

  {
    eml::expected<int, int> e;
    REQUIRE(std::is_default_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_constructible<decltype(e)>::value);
    REQUIRE(std::is_move_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_assignable<decltype(e)>::value);
    REQUIRE(std::is_move_assignable<decltype(e)>::value);
    REQUIRE(std::is_trivially_copy_constructible<decltype(e)>::value);
    REQUIRE(std::is_trivially_copy_assignable<decltype(e)>::value);
    REQUIRE(std::is_trivially_move_constructible<decltype(e)>::value);
    REQUIRE(std::is_trivially_move_assignable<decltype(e)>::value);
  }

  {
    eml::expected<int, std::string> e;
    REQUIRE(std::is_default_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_constructible<decltype(e)>::value);
    REQUIRE(std::is_move_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_assignable<decltype(e)>::value);
    REQUIRE(std::is_move_assignable<decltype(e)>::value);
    REQUIRE(!std::is_trivially_copy_constructible<decltype(e)>::value);
    REQUIRE(!std::is_trivially_copy_assignable<decltype(e)>::value);
    REQUIRE(!std::is_trivially_move_constructible<decltype(e)>::value);
    REQUIRE(!std::is_trivially_move_assignable<decltype(e)>::value);
  }

  {
    eml::expected<std::string, int> e;
    REQUIRE(std::is_default_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_constructible<decltype(e)>::value);
    REQUIRE(std::is_move_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_assignable<decltype(e)>::value);
    REQUIRE(std::is_move_assignable<decltype(e)>::value);
    REQUIRE(!std::is_trivially_copy_constructible<decltype(e)>::value);
    REQUIRE(!std::is_trivially_copy_assignable<decltype(e)>::value);
    REQUIRE(!std::is_trivially_move_constructible<decltype(e)>::value);
    REQUIRE(!std::is_trivially_move_assignable<decltype(e)>::value);
  }

  {
    eml::expected<std::string, std::string> e;
    REQUIRE(std::is_default_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_constructible<decltype(e)>::value);
    REQUIRE(std::is_move_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_assignable<decltype(e)>::value);
    REQUIRE(std::is_move_assignable<decltype(e)>::value);
    REQUIRE(!std::is_trivially_copy_constructible<decltype(e)>::value);
    REQUIRE(!std::is_trivially_copy_assignable<decltype(e)>::value);
    REQUIRE(!std::is_trivially_move_constructible<decltype(e)>::value);
    REQUIRE(!std::is_trivially_move_assignable<decltype(e)>::value);
  }
}
