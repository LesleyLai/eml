#include "compiler.hpp"

#include <catch2/catch.hpp>

namespace ast = eml::ast;

TEST_CASE("Type check on unary expressions")
{
  GIVEN("(- 2)")
  {
    std::unique_ptr<ast::AstNode> ast =
        ast::UnaryNegateExpr::create(ast::LiteralExpr::create(eml::Value{2.}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::Compiler{}.type_check(ast);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Number` as its type")
        {
          const auto& expr = dynamic_cast<ast::Expr&>(**checked_ast);
          REQUIRE(expr.type() == eml::NumberType{});
        }
      }
    }
  }

  GIVEN("(! 2)")
  {
    std::unique_ptr<ast::AstNode> ast =
        ast::UnaryNotExpr::create(ast::LiteralExpr::create(eml::Value{2.}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::Compiler{}.type_check(ast);
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
    std::unique_ptr<ast::AstNode> ast =
        ast::PlusOpExpr::create(ast::LiteralExpr::create(eml::Value{1.}),
                                ast::LiteralExpr::create(eml::Value{1.}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::Compiler{}.type_check(ast);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Number` as its type")
        {
          const auto& expr = dynamic_cast<ast::Expr&>(**checked_ast);
          REQUIRE(expr.type() == eml::NumberType{});
        }
      }
    }
  }

  GIVEN("(+ 1 true)")
  {
    std::unique_ptr<ast::AstNode> ast =
        ast::PlusOpExpr::create(ast::LiteralExpr::create(eml::Value{1.}),
                                ast::LiteralExpr::create(eml::Value{true}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::Compiler{}.type_check(ast);
      THEN("Failed the type check")
      {
        REQUIRE(!checked_ast.has_value());
      }
    }
  }

  GIVEN("(* 1 1)")
  {
    std::unique_ptr<ast::AstNode> ast =
        ast::MultOpExpr::create(ast::LiteralExpr::create(eml::Value{1.}),
                                ast::LiteralExpr::create(eml::Value{1.}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::Compiler{}.type_check(ast);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Number` as its type")
        {
          const auto& expr = dynamic_cast<ast::Expr&>(**checked_ast);
          REQUIRE(expr.type() == eml::NumberType{});
        }
      }
    }
  }

  GIVEN("(/ true 3)")
  {
    std::unique_ptr<ast::AstNode> ast =
        ast::DivOpExpr::create(ast::LiteralExpr::create(eml::Value{true}),
                               ast::LiteralExpr::create(eml::Value{3.}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::Compiler{}.type_check(ast);
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
    std::unique_ptr<ast::AstNode> ast =
        ast::EqOpExpr::create(ast::LiteralExpr::create(eml::Value{1.}),
                              ast::LiteralExpr::create(eml::Value{1.}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::Compiler{}.type_check(ast);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Bool` as its type")
        {
          const auto& expr = dynamic_cast<ast::Expr&>(**checked_ast);
          REQUIRE(expr.type() == eml::BoolType{});
        }
      }
    }
  }

  GIVEN("(== true false)")
  {
    std::unique_ptr<ast::AstNode> ast =
        ast::EqOpExpr::create(ast::LiteralExpr::create(eml::Value{true}),
                              ast::LiteralExpr::create(eml::Value{false}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::Compiler{}.type_check(ast);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Bool` as its type")
        {
          const auto& expr = dynamic_cast<ast::Expr&>(**checked_ast);
          REQUIRE(expr.type() == eml::BoolType{});
        }
      }
    }
  }

  GIVEN("(== 1 true)")
  {
    std::unique_ptr<ast::AstNode> ast =
        ast::PlusOpExpr::create(ast::LiteralExpr::create(eml::Value{1.}),
                                ast::LiteralExpr::create(eml::Value{true}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::Compiler{}.type_check(ast);
      THEN("Failed the type check")
      {
        REQUIRE(!checked_ast.has_value());
      }
    }
  }

  GIVEN("(!= 1 1)")
  {
    std::unique_ptr<ast::AstNode> ast =
        ast::NeqOpExpr::create(ast::LiteralExpr::create(eml::Value{1.}),
                               ast::LiteralExpr::create(eml::Value{1.}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::Compiler{}.type_check(ast);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Bool` as its type")
        {
          const auto& expr = dynamic_cast<ast::Expr&>(**checked_ast);
          REQUIRE(expr.type() == eml::BoolType{});
        }
      }
    }
  }

  GIVEN("(!= 1 true)")
  {
    std::unique_ptr<ast::AstNode> ast =
        ast::PlusOpExpr::create(ast::LiteralExpr::create(eml::Value{1.}),
                                ast::LiteralExpr::create(eml::Value{true}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::Compiler{}.type_check(ast);
      THEN("Failed the type check")
      {
        REQUIRE(!checked_ast.has_value());
      }
    }
  }

  GIVEN("(< 1 1)")
  {
    std::unique_ptr<ast::AstNode> ast =
        ast::LessOpExpr::create(ast::LiteralExpr::create(eml::Value{1.}),
                                ast::LiteralExpr::create(eml::Value{1.}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::Compiler{}.type_check(ast);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Bool` as its type")
        {
          const auto& expr = dynamic_cast<ast::Expr&>(**checked_ast);
          REQUIRE(expr.type() == eml::BoolType{});
        }
      }
    }
  }

  GIVEN("(< true true)")
  {
    std::unique_ptr<ast::AstNode> ast =
        ast::LessOpExpr::create(ast::LiteralExpr::create(eml::Value{true}),
                                ast::LiteralExpr::create(eml::Value{true}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::Compiler{}.type_check(ast);
      THEN("Failed the type check")
      {
        REQUIRE(!checked_ast.has_value());
      }
    }
  }

  GIVEN("(<= 1 1)")
  {
    std::unique_ptr<ast::AstNode> ast =
        ast::LeOpExpr::create(ast::LiteralExpr::create(eml::Value{1.}),
                              ast::LiteralExpr::create(eml::Value{1.}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::Compiler{}.type_check(ast);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Bool` as its type")
        {
          const auto& expr = dynamic_cast<ast::Expr&>(**checked_ast);
          REQUIRE(expr.type() == eml::BoolType{});
        }
      }
    }
  }

  GIVEN("(<= true true)")
  {
    std::unique_ptr<ast::AstNode> ast =
        ast::LeOpExpr::create(ast::LiteralExpr::create(eml::Value{true}),
                              ast::LiteralExpr::create(eml::Value{true}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::Compiler{}.type_check(ast);
      THEN("Failed the type check")
      {
        REQUIRE(!checked_ast.has_value());
      }
    }
  }

  GIVEN("(> 1 1)")
  {
    std::unique_ptr<ast::AstNode> ast =
        ast::GreaterOpExpr::create(ast::LiteralExpr::create(eml::Value{1.}),
                                   ast::LiteralExpr::create(eml::Value{1.}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::Compiler{}.type_check(ast);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Bool` as its type")
        {
          const auto& expr = dynamic_cast<ast::Expr&>(**checked_ast);
          REQUIRE(expr.type() == eml::BoolType{});
        }
      }
    }
  }

  GIVEN("(>= 1 1)")
  {
    std::unique_ptr<ast::AstNode> ast =
        ast::GeExpr::create(ast::LiteralExpr::create(eml::Value{1.}),
                            ast::LiteralExpr::create(eml::Value{1.}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::Compiler{}.type_check(ast);
      THEN("Passes the type check")
      {
        REQUIRE(checked_ast.has_value());

        THEN("Gets `Bool` as its type")
        {
          const auto& expr = dynamic_cast<ast::Expr&>(**checked_ast);
          REQUIRE(expr.type() == eml::BoolType{});
        }
      }
    }
  }

  GIVEN("(>= true true)")
  {
    std::unique_ptr<ast::AstNode> ast =
        ast::GeExpr::create(ast::LiteralExpr::create(eml::Value{true}),
                            ast::LiteralExpr::create(eml::Value{true}));
    WHEN("Type check")
    {
      const auto checked_ast = eml::Compiler{}.type_check(ast);
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
    eml::Compiler compiler;

    THEN("Type check should resolve the right hand side to type Bool")
    {
      const auto result = eml::parse(s1).and_then(
          [&compiler](auto&& ast) { return compiler.type_check(ast); });
      REQUIRE(result);
      const auto bind_type =
          dynamic_cast<ast::Definition&>(**result).binding_type();
      REQUIRE(bind_type.has_value());
      REQUIRE(*bind_type == eml::BoolType{});
    }

    GIVEN("x")
    {
      THEN("Should get the same type as the previous definition")
      {
        const auto result = eml::parse("x").and_then(
            [&compiler](auto&& ast) { return compiler.type_check(ast); });
        REQUIRE(result);
        const auto id_type =
            dynamic_cast<ast::IdentifierExpr&>(**result).type();
        REQUIRE(id_type.has_value());
        REQUIRE(*id_type == eml::BoolType{});
      }
    }
  }
}
