#include "ast.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "parser.hpp"

#include <catch2/catch.hpp>

TEST_CASE("Test parsing", "[parser]")
{
  eml::GarbageCollector gc{};

  GIVEN("(5 * 6)")
  {
    WHEN("Parsed")
    {
      const auto ast = eml::parse("(5 * 6)", gc);
      THEN("Should produces AST (* 5 6)")
      {
        REQUIRE(ast);
        REQUIRE(eml::to_string(**ast, eml::AstPrintOption::flat) == "(* 5 6)");
      }
    }
  }

  constexpr auto definition_str = "let x = 1 / 2 * 5";
  GIVEN(definition_str)
  {
    WHEN("Parsed")
    {
      const auto ast = eml::parse(definition_str, gc);
      THEN("Should produces AST (let x (* (/ 1 2) 5))")
      {
        REQUIRE(ast);
        REQUIRE(eml::to_string(**ast, eml::AstPrintOption::flat) ==
                "(let x (* (/ 1 2) 5))");
      }
    }
  }
}

TEST_CASE("Branching", "[parser]")
{
  eml::GarbageCollector gc{};

  GIVEN("An if-else branch")
  {
    constexpr auto branch_str = R"(
                                  if (x < 0) 0
                                  else {
                                    x
                                  }
                                  )";
    WHEN("Parsed")
    {
      const auto result = eml::parse(branch_str, gc);
      THEN("Should produces the correct AST")
      {
        REQUIRE(result);
        result.map([](auto& ast) {
          const auto& branch_node = dynamic_cast<eml::IfExpr&>(*ast);
          REQUIRE(eml::to_string(branch_node, eml::AstPrintOption::flat) ==
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
      const auto result = eml::parse(branch_str, gc);
      THEN("Should produces the correct AST")
      {
        REQUIRE(result);
        result.map([](auto& ast) {
          REQUIRE(eml::to_string(*ast, eml::AstPrintOption::flat) ==
                  "(if (< x 0) 0 (if (< x 5) 5 x))");
        });
      }
    }
  }
}

TEST_CASE("Function definitions")
{
  eml::GarbageCollector gc{};

  GIVEN("A lambda expression")
  {
    constexpr auto s1 = R"(\x -> x + 1)";

    WHEN("parsed")
    {
      const auto result = eml::parse(s1, gc);
      THEN("produces the correct AST")
      {
        REQUIRE(result);
        result.map([](auto& ast) {
          REQUIRE(eml::to_string(*ast, eml::AstPrintOption::flat) ==
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
      const auto result = eml::parse(s1, gc);
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
      const auto result = eml::parse(s1, gc);
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
      const auto result = eml::parse(s1, gc);
      THEN("produces the correct AST")
      {
        REQUIRE(result);
        result.map([](auto& ast) {
          REQUIRE(eml::to_string(*ast, eml::AstPrintOption::flat) ==
                  "(let f (lambda x (+ x 1)))");
        });
      }
    }
  }
}

TEST_CASE("Error handling of the parser", "[parser]")
{
  eml::GarbageCollector gc{};

  GIVEN("1!!!")
  {
    const auto ast = eml::parse("1!!!", gc);
    THEN("Should produces error")
    {
      REQUIRE(!ast.has_value());
    }
  }
}
