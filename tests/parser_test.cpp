#include "../src/parser.hpp"

#include <catch2/catch.hpp>
TEST_CASE("Test parsing")
{
  GIVEN("(5 * 6)")
  {
    const auto bytecode = *eml::compile("(5 * 6)");
    WHEN("Evaluated")
    {
      eml::VM vm{bytecode};
      const auto value = vm.interpret();
      THEN("Should produces result 30")
      {
        REQUIRE(value.unsafe_as_number() == Approx{30});
      }
    }
  }

  GIVEN("(5 * 2 + 3) * 4")
  {
    const auto bytecode = *eml::compile("(5 * 2 + 3) * 4");
    WHEN("Evaluated")
    {
      eml::VM vm{bytecode};
      const auto value = vm.interpret();
      THEN("Should produces result 52")
      {
        REQUIRE(value.unsafe_as_number() == Approx{52});
      }
    }
  }
}

// TEST_CASE("Error handling of the parser")
//{
//  GIVEN("1!!!")
//  {
//    const auto output = eml::compile("1!!!");
//    THEN("Should produces error")
//    {
//      REQUIRE(output == std::nullopt);
//    }
//  }
//}
