#include "vm.hpp"

#include <catch2/catch.hpp>

#include "util.hpp"

TEST_CASE("Arithmatic instructions", "[eml.vm]")
{
  using eml::chunk;
  using eml::line_num;
  using eml::Value;

  GIVEN("(((2 3 +) 4 /) (2 5 *) -)")
  {
    const auto v1 = 2.;
    const auto v2 = 3.;
    const auto v3 = 4.;
    const auto v4 = 2.;
    const auto v5 = 5.;

    chunk code;
    push_number(code, v1, line_num{0});
    push_number(code, v2, line_num{0});

    code.write(eml::op_add, line_num{0});

    push_number(code, v3, line_num{0});

    code.write(eml::op_divide, line_num{0});

    push_number(code, v4, line_num{0});
    push_number(code, v5, line_num{0});

    code.write(eml::op_multiply, line_num{0});
    code.write(eml::op_subtract, line_num{0});

    eml::VM machine{code};

    THEN("Evaluate to -8.75")
    {
      const double expected = -8.75;
      const auto result = machine.interpret();
      REQUIRE(result);
      REQUIRE(result->unsafe_as_number() == Approx(expected));
    }
  }
}
