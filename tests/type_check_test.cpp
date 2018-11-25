#include "type_checker.hpp"

#include <catch2/catch.hpp>

namespace ast = eml::ast;

TEST_CASE("Type check on binary expressions")
{
  GIVEN("(+ 1 1)")
  {
    ast::Expr_ptr ast =
        ast::PlusOpExpr::create(ast::LiteralExpr::create(eml::Value{1.}),
                                ast::LiteralExpr::create(eml::Value{1.}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::type_check(ast);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Number` as its type")
        {
          REQUIRE((*checked_ast)->type() == eml::NumberType{});
        }
      }
    }
  }

  GIVEN("(+ 1 true)")
  {
    ast::Expr_ptr ast =
        ast::PlusOpExpr::create(ast::LiteralExpr::create(eml::Value{1.}),
                                ast::LiteralExpr::create(eml::Value{true}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::type_check(ast);
      THEN("Failed the type check")
      {
        REQUIRE(!checked_ast.has_value());
      }
    }
  }

  GIVEN("(* 1 1)")
  {
    ast::Expr_ptr ast =
        ast::MultOpExpr::create(ast::LiteralExpr::create(eml::Value{1.}),
                                ast::LiteralExpr::create(eml::Value{1.}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::type_check(ast);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Number` as its type")
        {
          REQUIRE((*checked_ast)->type() == eml::NumberType{});
        }
      }
    }
  }

  GIVEN("(/ true 3)")
  {
    ast::Expr_ptr ast =
        ast::DivOpExpr::create(ast::LiteralExpr::create(eml::Value{true}),
                               ast::LiteralExpr::create(eml::Value{3.}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::type_check(ast);
      THEN("Failed the type check")
      {
        REQUIRE(!checked_ast.has_value());
      }
    }
  }
}
