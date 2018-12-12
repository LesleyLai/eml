#include "ast.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "parser.hpp"

#include <iostream>

#include <catch2/catch.hpp>

using eml::ast::PrintOption;

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
        REQUIRE(eml::to_string(**ast, PrintOption::flat) == "(* 5 6)");
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
        REQUIRE(eml::to_string(**ast, PrintOption::flat) ==
                "(let x (* (/ 1 2) 5))");
      }
    }
  }
}

TEST_CASE("Branching", "[parser]")
{
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
          REQUIRE(eml::to_string(branch_node, PrintOption::flat) ==
                  "(if (< x 0) 0 x)");
        });
      }
    }
  }

  GIVEN("An if-else if-else branch")
  {
    constexpr auto branch_str = R"(
                                    if (x < 0) {
                                      0
                                    } else if (x < 5) {
                                      5
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
          REQUIRE(eml::to_string(*ast, PrintOption::flat) ==
                  "(if (< x 0) 0 (if (< x 5) 5 x))");
        });
      }
    }
  }
}

TEST_CASE("Function definitions")
{
  GIVEN("A lambda expression")
  {
    constexpr auto s1 = R"(\x -> x + 1)";

    WHEN("parsed")
    {
      const auto result = eml::parse(s1);
      THEN("produces the correct AST")
      {
        REQUIRE(result);
        result.map([](auto& ast) {
          REQUIRE(eml::to_string(*ast, PrintOption::flat) ==
                  "(lambda x (+ x 1))");
        });
      }
    }
  }

  GIVEN("A lambda expression with no argument")
  {
    constexpr auto s1 = R"(\ -> x + 1)";

    WHEN("parsed")
    {
      const auto result = eml::parse(s1);
      THEN("Generate an error")
      {
        REQUIRE(!result.has_value());
      }
    }
  }

  GIVEN("A malformed lambda expression")
  {
    constexpr auto s1 = R"(\ x y x + x)";

    WHEN("parsed")
    {
      const auto result = eml::parse(s1);
      THEN("Generate an error")
      {
        REQUIRE(!result.has_value());
      }
    }
  }

  GIVEN("A binding to lambda expression")
  {
    constexpr auto s1 = R"(let f = \x -> x + 1)";

    WHEN("parsed")
    {
      const auto result = eml::parse(s1);
      THEN("produces the correct AST")
      {
        REQUIRE(result);
        result.map([](auto& ast) {
          REQUIRE(eml::to_string(*ast, PrintOption::flat) ==
                  "(let f (lambda x (+ x 1)))");
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
