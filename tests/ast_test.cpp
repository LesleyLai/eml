#include "ast.hpp"
#include "compiler.hpp"
#include "debug.hpp"

#include "vm_test_util.hpp"

#include <catch2/catch.hpp>

TEST_CASE("AST visiting and printing")
{
  GIVEN("A Unary negate expression -10")
  {
    WHEN("Invoke the Ast Printer")
    {
      THEN("Should print (- 10)")
      {
        eml::UnaryNegateExpr expr{eml::LiteralExpr::create(eml::Value{10.})};

        REQUIRE(eml::to_string(expr) == "(- 10)");
      }
    }
  }

  GIVEN("A binary arithmatic expression 3 * (4 + 5) / (-3 - 1)")
  {
    const eml::Expr_ptr expr = eml::DivOpExpr::create(
        eml::MultOpExpr::create(
            eml::LiteralExpr::create(eml::Value{3.}),
            eml::PlusOpExpr::create(eml::LiteralExpr::create(eml::Value{4.}),
                                    eml::LiteralExpr::create(eml::Value{5.}))),
        eml::MinusOpExpr::create(eml::UnaryNegateExpr::create(
                                     eml::LiteralExpr::create(eml::Value{3.})),
                                 eml::LiteralExpr::create(eml::Value{1.})));
    WHEN("Invoke the Ast Printer")
    {
      const auto str = eml::to_string(*expr);
      THEN("Should print the correct prefix notation")
      {
        REQUIRE(str == "(/ (* 3 (+ 4 5)) (- (- 3) 1))");
      }
    }

    WHEN("Compile into Bytecode")
    {
      eml::GarbageCollector gc{};
      eml::VM vm{};
      eml::Compiler compiler{gc};
      const auto [c, _] = compiler.generate_code(*expr);
      THEN("Should produces the expected instruction sets")
      {
        eml::Bytecode expected;
        push_number(expected, 3.);
        push_number(expected, 4.);
        push_number(expected, 5.);
        write_instruction(expected, eml::op_add_f64);
        write_instruction(expected, eml::op_multiply_f64);
        push_number(expected, 3.);
        write_instruction(expected, eml::op_negate_f64);
        push_number(expected, 1.);
        write_instruction(expected, eml::op_subtract_f64);
        write_instruction(expected, eml::op_divide_f64);

        REQUIRE(c.disassemble() == expected.disassemble());
      }

      THEN("Evaluate to -6.75")
      {
        const auto result = vm.interpret(c);
        REQUIRE(result);
        REQUIRE(result->is_number());
        REQUIRE(result->unsafe_as_number() == Approx(-6.75));
      }
    }
  }
}
