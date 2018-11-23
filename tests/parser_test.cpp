#include "ast.hpp"
#include "debug.hpp"
#include "parser.hpp"

#include <iostream>

#include <catch2/catch.hpp>
TEST_CASE("Test parsing")
{
  GIVEN("(5 * 6)")
  {
    WHEN("Parsed")
    {
      const auto ast = eml::parse("(5 * 6)");
      THEN("Should produces AST (* 5 6)")
      {
        REQUIRE(ast);
        REQUIRE(eml::string_from_ast(**ast) == "(* 5 6)");
      }
    }

    //    const auto bytecode = *eml::compile("(5 * 6)");
    //    WHEN("Evaluated")
    //    {
    //      eml::VM vm{bytecode};
    //      const auto value = vm.interpret();
    //      THEN("Should produces result 30")
    //      {
    //        REQUIRE(value.unsafe_as_number() == Approx{30});
    //      }
    //    }
  }
}

TEST_CASE("Error handling of the parser")
{
  GIVEN("1!!!")
  {
    const auto ast = eml::parse("1!!!");
    THEN("Should produces error")
    {
      REQUIRE(!ast.has_value());
      for (const auto& e : ast.error()) {
        std::clog << e.msg_ << '\n';
      }
    }
  }
}

/*

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

GIVEN("()")
{
  const auto bytecode = *eml::compile("()");
  WHEN("Evaluated")
  {
    eml::VM vm{bytecode};
    const auto value = vm.interpret();
    THEN("Should produces a unit value")
    {
      REQUIRE(value.is_unit());
    }
  }
}
}


*/
