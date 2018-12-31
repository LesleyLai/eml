#include "value.hpp"

#include <catch2/catch.hpp>
#include <functional>
#include <sstream>

TEST_CASE("Values' RTTI 'unsafe_as_xxx' and 'is_xxx' function")
{
  GIVEN("A unit value")
  {
    const eml::Value v{};

    THEN("Is unit value")
    {
      REQUIRE(v.is_unit());
    }

    THEN("Is not a number")
    {
      REQUIRE(!v.is_number());
    }

    THEN("Is not a boolean")
    {
      REQUIRE(!v.is_boolean());
    }
  }

  GIVEN("A double value")
  {
    const eml::Value v{1.25};

    THEN("Is not a unit value")
    {
      REQUIRE(!v.is_unit());
    }

    THEN("Is a number")
    {
      REQUIRE(v.is_number());
    }

    THEN("Is not a boolean")
    {
      REQUIRE(!v.is_boolean());
    }

    WHEN("Invoke unsafe_as_number")
    {
      THEN("Should produce the correct value")
      {
        REQUIRE(v.unsafe_as_number() == 1.25);
      }
    }
  }

  GIVEN("A boolean value")
  {
    const eml::Value v{true};

    THEN("Is not a unit value")
    {
      REQUIRE(!v.is_unit());
    }

    THEN("Is not a number")
    {
      REQUIRE(!v.is_number());
    }

    THEN("Is a boolean")
    {
      REQUIRE(v.is_boolean());
    }

    WHEN("Invoke unsafe_as_boolean")
    {
      THEN("Should produce the correct value")
      {
        REQUIRE(v.unsafe_as_boolean() == true);
      }
    }
  }
}

TEST_CASE("Value printing")
{
  std::stringstream ss;
  GIVEN("A double value")
  {
    const eml::Value v{0.5};

    THEN("Print as is")
    {
      REQUIRE(eml::to_string(eml::NumberType{}, v) == "0.5: Number");
    }
  }

  GIVEN("A unit value")
  {
    const eml::Value v{};

    THEN("Print (): Unit")
    {
      REQUIRE(eml::to_string(eml::UnitType{}, v) == "(): Unit");
    }
  }

  //  GIVEN("A string value")
  //  {
  //    const eml::Value v{};

  //    THEN("Print (): Unit")
  //    {
  //      REQUIRE(eml::to_string(eml::UnitType{}, v) == "(): Unit");
  //    }
  //  }
}
