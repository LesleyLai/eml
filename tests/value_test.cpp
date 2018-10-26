#include "../value.hpp"

#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Value printing")
{
  std::stringstream ss;
  GIVEN("An double value")
  {
    eml::value v{0.5};

    THEN("Print as is")
    {
      // using namespace eml;
      eml::operator<<(ss, v);
      REQUIRE(ss.str() == "0.5");
    }
  }
}
