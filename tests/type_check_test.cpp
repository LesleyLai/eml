#include "type_checker.hpp"

#include <catch2/catch.hpp>

namespace ast = eml::ast;

TEST_CASE("Type check on unary expressions")
{
  GIVEN("(- 2)")
  {
    ast::Expr_ptr ast =
        ast::UnaryNegateExpr::create(ast::LiteralExpr::create(eml::Value{2.}));
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

  GIVEN("(! 2)")
  {
    ast::Expr_ptr ast =
        ast::UnaryNotExpr::create(ast::LiteralExpr::create(eml::Value{2.}));
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

TEST_CASE("Type check on binary arithmatic expressions")
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

TEST_CASE("Type check on binary comparison expressions")
{
  GIVEN("(== 1 1)")
  {
    ast::Expr_ptr ast =
        ast::EqOpExpr::create(ast::LiteralExpr::create(eml::Value{1.}),
                              ast::LiteralExpr::create(eml::Value{1.}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::type_check(ast);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Bool` as its type")
        {
          REQUIRE((*checked_ast)->type() == eml::BoolType{});
        }
      }
    }
  }

  GIVEN("(== true false)")
  {
    ast::Expr_ptr ast =
        ast::EqOpExpr::create(ast::LiteralExpr::create(eml::Value{true}),
                              ast::LiteralExpr::create(eml::Value{false}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::type_check(ast);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Bool` as its type")
        {
          REQUIRE((*checked_ast)->type() == eml::BoolType{});
        }
      }
    }
  }

  GIVEN("(== 1 true)")
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

  GIVEN("(!= 1 1)")
  {
    ast::Expr_ptr ast =
        ast::NeqOpExpr::create(ast::LiteralExpr::create(eml::Value{1.}),
                               ast::LiteralExpr::create(eml::Value{1.}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::type_check(ast);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Bool` as its type")
        {
          REQUIRE((*checked_ast)->type() == eml::BoolType{});
        }
      }
    }
  }

  GIVEN("(!= 1 true)")
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

  GIVEN("(< 1 1)")
  {
    ast::Expr_ptr ast =
        ast::LessOpExpr::create(ast::LiteralExpr::create(eml::Value{1.}),
                                ast::LiteralExpr::create(eml::Value{1.}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::type_check(ast);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Bool` as its type")
        {
          REQUIRE((*checked_ast)->type() == eml::BoolType{});
        }
      }
    }
  }

  GIVEN("(< true true)")
  {
    ast::Expr_ptr ast =
        ast::LessOpExpr::create(ast::LiteralExpr::create(eml::Value{true}),
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

  GIVEN("(<= 1 1)")
  {
    ast::Expr_ptr ast =
        ast::LeOpExpr::create(ast::LiteralExpr::create(eml::Value{1.}),
                              ast::LiteralExpr::create(eml::Value{1.}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::type_check(ast);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Bool` as its type")
        {
          REQUIRE((*checked_ast)->type() == eml::BoolType{});
        }
      }
    }
  }

  GIVEN("(<= true true)")
  {
    ast::Expr_ptr ast =
        ast::LeOpExpr::create(ast::LiteralExpr::create(eml::Value{true}),
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

  GIVEN("(> 1 1)")
  {
    ast::Expr_ptr ast =
        ast::GreaterOpExpr::create(ast::LiteralExpr::create(eml::Value{1.}),
                                   ast::LiteralExpr::create(eml::Value{1.}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::type_check(ast);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Bool` as its type")
        {
          REQUIRE((*checked_ast)->type() == eml::BoolType{});
        }
      }
    }
  }

  GIVEN("(>= 1 1)")
  {
    ast::Expr_ptr ast =
        ast::GeExpr::create(ast::LiteralExpr::create(eml::Value{1.}),
                            ast::LiteralExpr::create(eml::Value{1.}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::type_check(ast);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Bool` as its type")
        {
          REQUIRE((*checked_ast)->type() == eml::BoolType{});
        }
      }
    }
  }

  GIVEN("(>= true true)")
  {
    ast::Expr_ptr ast =
        ast::GeExpr::create(ast::LiteralExpr::create(eml::Value{true}),
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
}
