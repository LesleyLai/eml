#include "ast.hpp"
#include "compiler.hpp"
#include "debug.hpp"

#include <catch2/catch.hpp>

void push_constant(eml::chunk& chunk, eml::Value value,
                   eml::line_num linum = eml::line_num{0})
{
  chunk.write(eml::op_push, linum);
  const auto offset = chunk.add_constant(value);
  chunk.write(eml::opcode{*offset}, linum);
}

TEST_CASE("AST visiting and printing")
{
  namespace ast = eml::ast;

  GIVEN("A Unary negate expression -10")
  {
    WHEN("Invoke the Ast Printer")
    {
      THEN("Should print (- 10)")
      {
        ast::UnaryNegateExpr expr{ast::LiteralExpr::create(eml::Value{10.})};

        REQUIRE(eml::string_from_ast(expr) == "(- 10)");
      }
    }
  }

  GIVEN("A binary arithmatic expression 3 * (4 + 5) / (-3 - 1)")
  {
    const ast::Expr_ptr expr = ast::DivOpExpr::create(
        ast::MultOpExpr::create(
            ast::LiteralExpr::create(eml::Value{3.}),
            ast::PlusOpExpr::create(ast::LiteralExpr::create(eml::Value{4.}),
                                    ast::LiteralExpr::create(eml::Value{5.}))),
        ast::MinusOpExpr::create(ast::UnaryNegateExpr::create(
                                     ast::LiteralExpr::create(eml::Value{3.})),
                                 ast::LiteralExpr::create(eml::Value{1.})));
    WHEN("Invoke the Ast Printer")
    {
      const auto str = eml::string_from_ast(*expr);
      THEN("Should print the correct prefix notation")
      {
        REQUIRE(str == "(/ (* 3 (+ 4 5)) (- (- 3) 1))");
      }
    }

    WHEN("Compile into Bytecode")
    {
      const auto c = eml::bytecode_from_ast(*expr);
      THEN("Should produces the expected instruction sets")
      {
        eml::chunk expected;
        push_constant(expected, eml::Value{3.});
        push_constant(expected, eml::Value{4.});
        push_constant(expected, eml::Value{5.});
        expected.write(eml::op_add, eml::line_num{0});
        expected.write(eml::op_multiply, eml::line_num{0});
        push_constant(expected, eml::Value{3.});
        expected.write(eml::op_negate, eml::line_num{0});
        push_constant(expected, eml::Value{1.});
        expected.write(eml::op_subtract, eml::line_num{0});
        expected.write(eml::op_divide, eml::line_num{0});

        REQUIRE(c.disassemble() == expected.disassemble());
      }

      THEN("Evaluate to -6.75")
      {
        eml::VM vm{c};
        eml::Value value = vm.interpret();
        REQUIRE(value.is_number());
        REQUIRE(value.unsafe_as_number() == Approx(-6.75));
      }
    }
  }
}