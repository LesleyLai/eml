#include <catch2/catch.hpp>

#include <array>

#include "../scanner.hpp"

TEST_CASE("Scanner")
{
  using namespace eml;

  GIVEN("A string of keywords")
  {
    const auto keywords = R"(and case class def else extern false for if let not
        or print return this true variant)";

    scanner s{keywords};

    WHEN("Scan the string")
    {
      THEN("Should get corerect keywords tokens")
      {
        std::array expected{
            token{token_type::keyword_and, "and"},
            token{token_type::keyword_case, "case"},
            token{token_type::keyword_class, "class"},
            token{token_type::keyword_def, "def"},
            token{token_type::keyword_else, "else"},
            token{token_type::keyword_extern, "extern"},
            token{token_type::keyword_false, "false"},
            token{token_type::keyword_for, "for"},
            token{token_type::keyword_if, "if"},
            token{token_type::keyword_let, "let"},
            token{token_type::keyword_not, "not"},
            token{token_type::keyword_or, "or"},
            token{token_type::keyword_print, "print"},
            token{token_type::keyword_return, "return"},
            token{token_type::keyword_this, "this"},
            token{token_type::keyword_true, "true"},
            token{token_type::keyword_variant, "variant"},
        };

        auto expected_itr = std::begin(expected);
        for (auto token : s) {
          CHECK(token == *expected_itr);
          ++expected_itr;
        }
      }
    }
  }
}

TEST_CASE("Error handling of the scanner")
{
  using namespace eml;

  GIVEN("A string contains unknown characters")
  {
    scanner s{R"(let # = 3)"};
    WHEN("Scan the string")
    {
      THEN("Correct identify the error, but scan the remaining part correctly")
      {
        std::array expected{
            token{token_type::keyword_let, "let"},
            token{token_type::error, "Unexpected character."},
            token{token_type::equal, "="},
            token{token_type::number_literal, "3"},
        };

        auto expected_itr = std::begin(expected);
        for (auto token : s) {
          CHECK(token == *expected_itr);
          ++expected_itr;
        }
      }
    }

    GIVEN("None-terminate string")
    {
      scanner s{R"("Hello world)"};
      THEN("Should detect the problem")
      {
        token expected{token_type::error, "Unterminated string."};
        REQUIRE(*s.begin() == expected);
      }
    }
  }
}
