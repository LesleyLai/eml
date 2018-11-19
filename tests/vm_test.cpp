#include "../src/vm.hpp"

#include <catch2/catch.hpp>

using namespace eml;

void push_constant(eml::chunk& chunk, eml::value value, eml::line_num linum)
{
  chunk.write(op_push, linum);
  const auto offset = chunk.add_constant(value);
  chunk.write(eml::opcode{*offset}, linum);
}

TEST_CASE("Manually write instructions")
{
  SECTION("Arithmatic instructions")
  {
    GIVEN("(((2 3 +) 4 /) (2 5 *) -)")
    {
      const auto v1 = value{2.};
      const auto v2 = value{3.};
      const auto v3 = value{4.};
      const auto v4 = value{2.};
      const auto v5 = value{5.};

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
        double expected = *(((v1 + v2) / v3) - (v4 * v5)).as<double>();
        double result = *machine.interpret().as<double>();
        REQUIRE(result == Approx(expected));
      }
    }
  }
}
