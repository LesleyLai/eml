#include "../value.hpp"

#include <catch2/catch.hpp>
#include <functional>
#include <sstream>

TEST_CASE("Value printing")
{
  std::stringstream ss;
  GIVEN("A double value")
  {
    const eml::value v{0.5};

    THEN("Print as is")
    {
      eml::operator<<(ss, v);
      REQUIRE(ss.str() == "0.5");
    }
  }

  //  GIVEN("A unit value")
  //  {
  //    const eml::value v{eml::unit_type{}};

  //    THEN("Print Unit")
  //    {
  //      eml::operator<<(ss, v);
  //      REQUIRE(ss.str() == "Unit");
  //    }
  //  }
}

TEST_CASE("Arithmatics")
{
  GIVEN("Two double value v1=1.1 and v2=2.2")
  {
    const eml::value v1{1.1};
    const eml::value v2{2.2};

    THEN("-v1 = -1.1")
    {
      const double result = -1.1;
      REQUIRE(eml::operator-(v1).val.num == Approx(result));
    }

    THEN("v1 + v2 = 3.3")
    {
      const double result = 3.3;
      REQUIRE(eml::operator+(v1, v2).val.num == Approx(result));
    }

    THEN("v1 - v2 = -1.1")
    {
      const double result = -1.1;
      REQUIRE(eml::operator-(v1, v2).val.num == Approx(result));
    }

    THEN("v1 * v2 = 2.42")
    {
      const double result = 2.42;
      REQUIRE(eml::operator*(v1, v2).val.num == Approx(result));
    }

    THEN("v1 / v2 = 0.5")
    {
      const double result = 0.5;
      REQUIRE(eml::operator/(v1, v2).val.num == Approx(result));
    }
  }
}
