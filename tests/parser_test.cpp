#include "ast.hpp"
#include "debug.hpp"
#include "error.hpp"
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
    }
  }
}
