#include <catch2/catch.hpp>

#include "eml.hpp"
#include "string.hpp"

TEST_CASE("String tests", "[string]")
{
  eml::GarbageCollector gc;
  eml::Compiler compiler{gc};
  eml::VM vm{gc};

  GIVEN("String concatenation")
  {

    compiler
        .compile(R"(
                       "hello" ++ " " ++ "world"
                       )")
        .map([&vm](const auto& compiler_output) {
          const auto [bytecode, type] = compiler_output;
          const auto result = vm.interpret(bytecode);

          REQUIRE(result);
          REQUIRE(eml::to_string(eml::StringType{}, *result,
                                 eml::PrintType::no) == "\"hello world\"");
        })
        .map_error([](const auto&) { FAIL("File to compile "); });
  }
}
