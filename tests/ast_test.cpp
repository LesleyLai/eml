#include "ast.hpp"
#include "debug.hpp"

#include <catch2/catch.hpp>

TEST_CASE("AST visiting and printing")
{
  namespace ast = eml::ast;

  GIVEN("A Unary negate expression -10")
  {
    WHEN("Invoke the Ast Printer")
    {
      THEN("Should print (- 10)")
      {
        ast::UnaryNegateExpr expr{ast::ConstExpr::create(eml::Value{10.})};

        REQUIRE(eml::string_from_ast(expr) == "(- 10)");
      }
    }
  }

  GIVEN("A binary arithmatic expression 3 * (4 + 5) / (-3 - 1)")
  {
    const ast::Expr_ptr expr = ast::DivOpExpr::create(
        ast::MultOpExpr::create(
            ast::ConstExpr::create(eml::Value{3.}),
            ast::PlusOpExpr::create(ast::ConstExpr::create(eml::Value{4.}),
                                    ast::ConstExpr::create(eml::Value{5.}))),
        ast::MinusOpExpr::create(ast::UnaryNegateExpr::create(
                                     ast::ConstExpr::create(eml::Value{3.})),
                                 ast::ConstExpr::create(eml::Value{1.})));
    WHEN("Invoke the Ast Printer")
    {
      THEN("Should print the correct prefix notation")
      {
        REQUIRE(eml::string_from_ast(*expr) == "(/ (* 3 (+ 4 5)) (- (- 3) 1))");
      }
    }
  }
}
