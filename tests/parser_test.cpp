#include "ast.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "parser.hpp"

#include <iostream>

#include <catch2/catch.hpp>
TEST_CASE("Test parsing", "[parser]")
{
  GIVEN("(5 * 6)")
  {
    WHEN("Parsed")
    {
      const auto ast = eml::parse("(5 * 6)");
      THEN("Should produces AST (* 5 6)")
      {
        REQUIRE(ast);
        REQUIRE(eml::to_string(**ast) == "(* 5 6)");
      }
    }
  }

  constexpr auto definition_str = "let x = 1 / 2 * 5";
  GIVEN(definition_str)
  {
    WHEN("Parsed")
    {
      const auto ast = eml::parse(definition_str);
      THEN("Should produces AST (let x (* (/ 1 2) 5))")
      {
        REQUIRE(ast);
        REQUIRE(eml::to_string(**ast) == "(let x (* (/ 1 2) 5))");
      }
    }
  }

  GIVEN("An if-else branch")
  {
    constexpr auto branch_str = R"(
                                  if (x < 0) {
                                    0
                                  } else {
                                    x
                                  }
                                  )";
    WHEN("Parsed")
    {
      const auto result = eml::parse(branch_str);
      THEN("Should produces the correct AST")
      {
        REQUIRE(result);
        result.map([](auto& ast) {
          const auto& branch_node = dynamic_cast<eml::ast::IfExpr&>(*ast);
          REQUIRE(eml::to_string(branch_node.cond()) == "(< x 0)");
          REQUIRE(eml::to_string(branch_node.If()) == "0");
          REQUIRE(eml::to_string(branch_node.Else()) == "x");
        });
      }
    }
  }
}

TEST_CASE("Error handling of the parser", "[parser]")
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
