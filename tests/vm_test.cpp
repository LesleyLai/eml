#include "vm.hpp"

#include <ApprovalTests.hpp>
#include <catch2/catch.hpp>

#include "vm_test_util.hpp"

TEST_CASE("Arithmatic instructions", "[eml.vm]")
{
  using eml::Bytecode;
  using eml::line_num;
  using eml::Value;

  GIVEN("(((2 3 +) 4 /) (2 5 *) -)")
  {
    const auto v1 = 2.;
    const auto v2 = 3.;
    const auto v3 = 4.;
    const auto v4 = 2.;
    const auto v5 = 5.;

    Bytecode code;
    push_number(code, v1);
    push_number(code, v2);

    write_instruction(code, eml::op_add_f64);

    push_number(code, v3);

    write_instruction(code, eml::op_divide_f64);

    push_number(code, v4);
    push_number(code, v5);

    write_instruction(code, eml::op_multiply_f64);
    write_instruction(code, eml::op_subtract_f64);

    eml::GarbageCollector gc{};
    eml::VM machine{gc};

    THEN("Evaluate to -8.75")
    {
      const double expected = -8.75;
      const auto result = machine.interpret(code);
      REQUIRE(result);
      REQUIRE(result->unsafe_as_number() == Approx(expected));
    }
  }
}

TEST_CASE("Jumps", "[eml.vm]")
{
  using eml::Bytecode;
  using eml::line_num;
  using eml::Value;

  GIVEN("(if (> 5 1) (+ 2 3) (- 4 6)) 2")
  {
    const auto v1 = 5.;
    const auto v2 = 1.;
    const auto v3 = 2.;
    const auto v4 = 3.;
    const auto v5 = 4.;
    const auto v6 = 6.;

    Bytecode code;
    push_number(code, v1);
    push_number(code, v2);
    write_instruction(code, eml::op_greater_f64);
    write_jump(code, eml::op_jmp_false, 7);
    push_number(code, v3);                         // 2
    push_number(code, v4);                         // 2
    write_instruction(code, eml::op_add_f64);      // 1
    write_jump(code, eml::op_jmp, 5);              // 2
    push_number(code, v5);                         // 2
    push_number(code, v6);                         // 2
    write_instruction(code, eml::op_subtract_f64); // 1

    eml::GarbageCollector gc{};
    eml::VM machine{gc};

    THEN("Evaluate to 5")
    {
      const double expected = 5;
      const auto result = machine.interpret(code);
      REQUIRE(result);
      REQUIRE(result->unsafe_as_number() == Approx(expected));
    }
  }

  GIVEN("(if (< 5 1) (+ 2 3) (- 4 6))")
  {
    const auto v1 = 5.;
    const auto v2 = 1.;
    const auto v3 = 2.;
    const auto v4 = 3.;
    const auto v5 = 4.;
    const auto v6 = 6.;

    Bytecode code;
    push_number(code, v1);
    push_number(code, v2);
    write_instruction(code, eml::op_less_f64);
    write_jump(code, eml::op_jmp_false, 7);
    push_number(code, v3);                         // 2
    push_number(code, v4);                         // 2
    write_instruction(code, eml::op_add_f64);      // 1
    write_jump(code, eml::op_jmp, 5);              // 2
    push_number(code, v5);                         // 2
    push_number(code, v6);                         // 2
    write_instruction(code, eml::op_subtract_f64); // 1

    eml::GarbageCollector gc{};
    eml::VM machine{gc};

    THEN("Evaluate to -2")
    {
      const double expected = -2;
      const auto result = machine.interpret(code);
      REQUIRE(result);
      REQUIRE(result->unsafe_as_number() == Approx(expected));
    }
  }
}
