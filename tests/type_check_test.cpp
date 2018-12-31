#include "ast.hpp"
#include "compiler.hpp"

#include <memory_resource>

#include <catch2/catch.hpp>

auto parse_and_type_check(eml::Compiler& compiler, std::string_view s,
                          eml::GarbageCollector& gc)
{
  return eml::parse(s, gc).and_then(
      [&compiler](auto&& ast) { return compiler.type_check(ast); });
}

TEST_CASE("Type check on unary expressions")
{
  eml::GarbageCollector gc{*std::pmr::new_delete_resource()};
  eml::Compiler compiler{gc};

  GIVEN("- 2")
  {
    WHEN("Type check")
    {
      const auto checked_ast = parse_and_type_check(compiler, "-2", gc);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Number` as its type")
        {
          const auto& expr = dynamic_cast<eml::Expr&>(**checked_ast);
          REQUIRE(eml::match(expr.type(), eml::NumberType{}));
        }
      }
    }
  }

  GIVEN("(! 2)")
  {
    WHEN("Type check")
    {
      const auto checked_ast = parse_and_type_check(compiler, "!2", gc);
      THEN("Failed the type check")
      {
        REQUIRE(!checked_ast.has_value());
      }
    }
  }
}

TEST_CASE("Type check on binary arithmatic expressions")
{
  eml::GarbageCollector gc{*std::pmr::new_delete_resource()};
  eml::Compiler compiler{gc};

  GIVEN("(+ 1 1)")
  {
    WHEN("Type check")
    {
      const auto checked_ast = parse_and_type_check(compiler, "1 + 1", gc);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Number` as its type")
        {
          const auto& expr = dynamic_cast<eml::Expr&>(**checked_ast);
          REQUIRE(eml::match(expr.type(), eml::NumberType{}));
        }
      }
    }
  }

  GIVEN("(+ 1 true)")
  {
    WHEN("Type check")
    {
      const auto checked_ast = parse_and_type_check(compiler, "1 + true", gc);
      THEN("Failed the type check")
      {
        REQUIRE(!checked_ast.has_value());
      }
    }
  }

  GIVEN("(* 1 1)")
  {
    WHEN("Type check")
    {
      const auto checked_ast = parse_and_type_check(compiler, "1 * 1", gc);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Number` as its type")
        {
          const auto& expr = dynamic_cast<eml::Expr&>(**checked_ast);
          REQUIRE(eml::match(expr.type(), eml::NumberType{}));
        }
      }
    }
  }

  GIVEN("(/ true 3)")
  {
    WHEN("Type check")
    {
      const auto checked_ast = parse_and_type_check(compiler, "true / 3", gc);
      THEN("Failed the type check")
      {
        REQUIRE(!checked_ast.has_value());
      }
    }
  }
}

TEST_CASE("Type check on binary comparison expressions")
{
  eml::GarbageCollector gc{*std::pmr::new_delete_resource()};
  eml::Compiler compiler{gc};

  GIVEN("(== 1 1)")
  {
    WHEN("Type check")
    {
      const auto checked_ast = parse_and_type_check(compiler, "1 == 1", gc);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Bool` as its type")
        {
          const auto& expr = dynamic_cast<eml::Expr&>(**checked_ast);
          REQUIRE(eml::match(expr.type(), eml::BoolType{}));
        }
      }
    }
  }

  GIVEN("(== true false)")
  {
    WHEN("Type check")
    {
      const auto checked_ast =
          parse_and_type_check(compiler, "true == false", gc);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Bool` as its type")
        {
          const auto& expr = dynamic_cast<eml::Expr&>(**checked_ast);
          REQUIRE(eml::match(expr.type(), eml::BoolType{}));
        }
      }
    }
  }

  GIVEN("(== 1 true)")
  {
    WHEN("Type check")
    {
      const auto checked_ast = parse_and_type_check(compiler, "1 == true", gc);
      THEN("Failed the type check")
      {
        REQUIRE(!checked_ast.has_value());
      }
    }
  }

  GIVEN("(!= 1 1)")
  {
    WHEN("Type check")
    {
      const auto checked_ast = parse_and_type_check(compiler, "1 != 1", gc);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Bool` as its type")
        {
          const auto& expr = dynamic_cast<eml::Expr&>(**checked_ast);
          REQUIRE(eml::match(expr.type(), eml::BoolType{}));
        }
      }
    }
  }

  GIVEN("(!= 1 true)")
  {
    WHEN("Type check")
    {
      const auto checked_ast = parse_and_type_check(compiler, "1 != true", gc);
      THEN("Failed the type check")
      {
        REQUIRE(!checked_ast.has_value());
      }
    }
  }

  GIVEN("(< 1 1)")
  {
    WHEN("Type check")
    {
      const auto checked_ast = parse_and_type_check(compiler, "1 < 1", gc);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Bool` as its type")
        {
          const auto& expr = dynamic_cast<eml::Expr&>(**checked_ast);
          REQUIRE(eml::match(expr.type(), eml::BoolType{}));
        }
      }
    }
  }

  GIVEN("(< true true)")
  {

    WHEN("Type check")
    {
      const auto checked_ast =
          parse_and_type_check(compiler, "true < true", gc);
      THEN("Failed the type check")
      {
        REQUIRE(!checked_ast.has_value());
      }
    }
  }

  GIVEN("(<= 1 1)")
  {
    WHEN("Type check")
    {
      const auto checked_ast = parse_and_type_check(compiler, "1 <= 1", gc);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());
        THEN("Gets `Bool` as its type")
        {
          const auto& expr = dynamic_cast<eml::Expr&>(**checked_ast);
          REQUIRE(eml::match(expr.type(), eml::BoolType{}));
        }
      }
    }
  }

  GIVEN("(<= true true)")
  {
    WHEN("Type check")
    {
      const auto checked_ast =
          parse_and_type_check(compiler, "true <= true", gc);
      THEN("Failed the type check")
      {
        REQUIRE(!checked_ast.has_value());
      }
    }
  }

  GIVEN("(> 1 1)")
  {
    WHEN("Type check")
    {
      const auto checked_ast = parse_and_type_check(compiler, "1 > 1", gc);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Bool` as its type")
        {
          const auto& expr = dynamic_cast<eml::Expr&>(**checked_ast);
          REQUIRE(eml::match(expr.type(), eml::BoolType{}));
        }
      }
    }
  }

  GIVEN("(>= 1 1)")
  {
    WHEN("Type check")
    {
      const auto checked_ast = parse_and_type_check(compiler, "1 >= 1", gc);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Bool` as its type")
        {
          const auto& expr = dynamic_cast<eml::Expr&>(**checked_ast);
          REQUIRE(eml::match(expr.type(), eml::BoolType{}));
        }
      }
    }
  }

  GIVEN("(>= true true)")
  {
    WHEN("Type check")
    {
      const auto checked_ast =
          parse_and_type_check(compiler, "true >= true", gc);
      THEN("Failed the type check")
      {
        REQUIRE(!checked_ast.has_value());
      }
    }
  }
}

TEST_CASE("Correctly type infer the let declerations and identifiers")
{
  constexpr auto s1 = "let x = true";
  GIVEN(s1)
  {
    eml::GarbageCollector gc{*std::pmr::new_delete_resource()};
    eml::Compiler compiler{gc};

    THEN("Type check should resolve the right hand side to type Bool")
    {
      const auto result = parse_and_type_check(compiler, s1, gc);
      REQUIRE(result);
      const auto bind_type =
          dynamic_cast<eml::Definition&>(**result).binding_type();
      REQUIRE(bind_type.has_value());
      REQUIRE(eml::match(*bind_type, eml::BoolType{}));

      const auto result2 = parse_and_type_check(compiler, "x", gc);
      REQUIRE(result2.has_value());

      const auto& id = dynamic_cast<eml::IdentifierExpr&>(**result2);

      REQUIRE(id.has_type());
      REQUIRE(eml::match(id.type(), eml::BoolType{}));
    }
  }
}

TEST_CASE("Type check for branches")
{
  eml::GarbageCollector gc{*std::pmr::new_delete_resource()};
  eml::Compiler compiler{gc};

  GIVEN("An if expression")
  {
    WHEN("Type check")
    {
      const auto checked_ast =
          parse_and_type_check(compiler, "if (true) { 2 } else {3}", gc);
      THEN("Resolve to the type of the branches")
      {
        REQUIRE(checked_ast.has_value());
        const auto& if_expr = dynamic_cast<eml::IfExpr&>(**checked_ast);
        REQUIRE(eml::match(if_expr.type(), if_expr.If().type()));
        REQUIRE(eml::match(if_expr.type(), if_expr.Else().type()));
      }
    }
  }

  GIVEN("A mistyped if expression condition")
  {
    WHEN("Type check")
    {
      const auto checked_ast =
          parse_and_type_check(compiler, "if (1) { 2 } else {3}", gc);
      THEN("Failed the type check")
      {
        REQUIRE(!checked_ast.has_value());
      }
    }
  }

  GIVEN("Mismatched if expression branches")
  {
    WHEN("Type check")
    {
      const auto checked_ast =
          parse_and_type_check(compiler, "if (1) { 2 } else {()}", gc);
      THEN("Failed the type check")
      {
        REQUIRE(!checked_ast.has_value());
      }
    }
  }
}
