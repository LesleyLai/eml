#include "../vm.hpp"

#include <catch2/catch.hpp>

using namespace eml;

TEST_CASE("Doing arithmatic instructions")
{
  GIVEN("(((2 3 +) 4 /) (2 5 *) -)")
  {
    const double v1 = 2;
    const double v2 = 3;
    const double v3 = 4;
    const double v4 = 2;
    const double v5 = 5;
    auto get_codes = [&]() {
      chunk code;
      code.write(op_push, line_num{0});
      code.add_constant(value{v1});

      code.write(op_push, line_num{0});
      code.add_constant(value{v2});

      code.write(op_add, line_num{0});

      code.write(op_push, line_num{0});
      code.add_constant(value{v3});

      code.write(op_divide, line_num{0});

      code.write(op_push, line_num{0});
      code.add_constant(value{v4});

      code.write(op_push, line_num{0});
      code.add_constant(value{v5});

      code.write(op_multiply, line_num{0});
      code.write(op_subtract, line_num{0});
      code.write(op_return, line_num{0});

      return code;
    };

    vm machine{get_codes()};
    THEN("Evaluate to -8.75")
    {
      double result = (((v1 + v2) / v3) - (v4 * v5));
      REQUIRE(machine.interpret().val.num == Approx(result));
    }
  }
}
