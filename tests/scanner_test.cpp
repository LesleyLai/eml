#include <catch2/catch.hpp>

#include <array>

#include "scanner.hpp"

using eml::Scanner;
using eml::Token;
using eml::token_type;

TEST_CASE("scanner", "[scanner]")
{
  GIVEN("hello")
  {
    const auto hello = "hello";
    THEN("Scan as an identifier")
    {
      Scanner s{hello};
      REQUIRE(s.begin() != s.end());
      REQUIRE(s.begin()->type == token_type::identifier);
    }
  }

  GIVEN("A string of keywords")
  {
    const auto keywords =
        R"(and async await case class def else extern false for if let not
        or print return this true () unsafe variant)";

    Scanner s{keywords};

    WHEN("Scan the string")
    {
      THEN("Should get corerect keywords tokens")
      {
        std::array expected{
            Token{token_type::keyword_and, "and"},
            Token{token_type::keyword_async, "async"},
            Token{token_type::keyword_await, "await"},
            Token{token_type::keyword_case, "case"},
            Token{token_type::keyword_class, "class"},
            Token{token_type::keyword_def, "def"},
            Token{token_type::keyword_else, "else"},
            Token{token_type::keyword_extern, "extern"},
            Token{token_type::keyword_false, "false"},
            Token{token_type::keyword_for, "for"},
            Token{token_type::keyword_if, "if"},
            Token{token_type::keyword_let, "let"},
            Token{token_type::keyword_not, "not"},
            Token{token_type::keyword_or, "or"},
            Token{token_type::keyword_print, "print"},
            Token{token_type::keyword_return, "return"},
            Token{token_type::keyword_this, "this"},
            Token{token_type::keyword_true, "true"},
            Token{token_type::keyword_unit, "()"},
            Token{token_type::keyword_unsafe, "unsafe"},
            Token{token_type::keyword_variant, "variant"},
        };

        auto scanner_itr = std::begin(s);

        for (auto expected_token : expected) {
          REQUIRE(*scanner_itr == expected_token);
          ++scanner_itr;
        }
        REQUIRE(scanner_itr == std::end(s));
      }
    }
  }

  GIVEN("A multi-line string")
  {
    const auto* str = R"("
                     Hello
                     world
                     ")";

    Scanner s{str};
    REQUIRE(s.begin() != s.end());

    THEN("The whole string is one token")
    {
      REQUIRE(str == s.begin()->text);
      REQUIRE(++s.begin() == s.end());
    }
  }

  SECTION("Ignore single line comments")
  {
    Scanner s{R"(// Hello world)"};
    REQUIRE(s.begin() == s.end());
  }
}

TEST_CASE("Error handling of the scanner", "[scanner]")
{
  GIVEN("A string contains unknown characters")
  {
    Scanner s{R"(let # = 3)"};
    WHEN("Scan the string")
    {
      THEN("Correct identify the error, but scan the remaining part correctly")
      {
        std::array expected{
            Token{token_type::keyword_let, "let"},
            Token{token_type::error, "Unexpected character."},
            Token{token_type::equal, "="},
            Token{token_type::number_literal, "3"},
        };

        auto scanner_itr = std::begin(s);
        for (auto expected_token : expected) {
          REQUIRE(*scanner_itr == expected_token);
          ++scanner_itr;
        }
        REQUIRE(scanner_itr == std::end(s));
      }
    }
  }

  GIVEN("None-terminate string")
  {
    Scanner s{R"("Hello world)"};
    THEN("Should detect the problem")
    {
      Token expected{token_type::error, "Unterminated string."};
      REQUIRE(*s.begin() == expected);
    }
  }
}
