#include "vm.hpp"

#include <catch2/catch.hpp>

#include "util.hpp"

using namespace eml;

TEST_CASE("Arithmatic instructions", "[eml.vm]")
{
  GIVEN("(((2 3 +) 4 /) (2 5 *) -)")
  {
    const auto v1 = Value{2.};
    const auto v2 = Value{3.};
    const auto v3 = Value{4.};
    const auto v4 = Value{2.};
    const auto v5 = Value{5.};

    chunk code;
    push_constant(code, v1, line_num{0});
    push_constant(code, v2, line_num{0});

    code.write(op_add, line_num{0});

    push_constant(code, v3, line_num{0});

    code.write(op_divide, line_num{0});

    push_constant(code, v4, line_num{0});
    push_constant(code, v5, line_num{0});

    code.write(op_multiply, line_num{0});
    code.write(op_subtract, line_num{0});

    VM machine{code};

    THEN("Evaluate to -8.75")
    {
      const double expected = -8.75;
      const double result = machine.interpret().unsafe_as_number();
      REQUIRE(result == Approx(expected));
    }
  }
}

TEST_CASE("Write and read global values", "[eml.vm]")
{
  GIVEN("let x = 2; x")
  {
    chunk code;
    const auto v1 = Value{2.};
    push_constant(code, v1, line_num{0});
    // push_constant(code, v1, line_num{0});
  }
}
