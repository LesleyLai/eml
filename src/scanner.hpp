#ifndef EML_SCANNER_HPP
#define EML_SCANNER_HPP

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string_view>

#include "common.hpp"

/**
 * @file scanner.hpp
 * @brief This file contains the scanner for the EML compiler
 */

namespace eml {

constexpr auto isalpha(char c) noexcept -> bool
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

constexpr auto isdigit(char c) noexcept -> bool
{
  return (c >= '0' && c <= '9');
}

std::ostream& operator<<(std::ostream& s, token_type t);
std::ostream& operator<<(std::ostream& s, Token t);

constexpr bool operator==(Token lhs, Token rhs)
{
  return lhs.type == rhs.type && lhs.text == rhs.text;
}

/// @brief The scanner scan the input string and output tokens
struct Scanner {
  std::string_view text;

  struct iterator {
    constexpr explicit iterator(std::string_view source) noexcept
        : start{source.data()}, current{start}, current_line_start{start}
    {
      ++(*this);
    }

    constexpr auto operator++() -> iterator&
    {
      token_ = next_token();
      return *this;
    }

    /**
     * @brief operator *
     * @warning If this iterator is not dereferenceable, operation is undefined
     */
    constexpr auto operator*() const -> const Token
    {
      return token_;
    }

    constexpr auto operator-> () const -> const Token*
    {
      return &token_;
    }

    constexpr auto operator==(const iterator rhs) const -> bool
    {
      return (**this == *rhs);
    }

    constexpr auto operator!=(const iterator rhs) const -> bool
    {
      return !(*this == rhs);
    }

  private:
    const char* start;
    const char* current;
    const char* current_line_start;
    std::size_t current_line = 1;
    Token token_; // The token when we direference

    constexpr auto next_token() -> Token
    {
      skip_whitespace();

      if (at_end()) {
        return Token{};
      }

      start = current;

      char c = advance();
      if (eml::isalpha(c)) {
        return identifier();
      }
      if (eml::isdigit(c)) {
        return number();
      }

      switch (c) {
      case '(':
        return make_token(match(')') ? token_type::keyword_unit
                                     : token_type::left_paren);
      case ')':
        return make_token(token_type::right_paren);
      case '{':
        return make_token(token_type::left_brace);
      case '}':
        return make_token(token_type::right_brace);
      case ':':
        return make_token(token_type::colon);
      case ';':
        return make_token(token_type::semicolon);
      case ',':
        return make_token(token_type::comma);
      case '.':
        return make_token(token_type::dot);
      case '-':
        return make_token(match('>') ? token_type::minus_right_arrow
                                     : token_type::minus);
      case '+':
        return make_token(token_type::plus);
      case '/':
        return make_token(token_type::slash);
      case '\\':
        return make_token(token_type::backslash);
      case '*':
        return make_token(token_type::star);

      case '!':
        return make_token(match('=') ? token_type::bang_equal
                                     : token_type::bang);
      case '=':
        if (match('=')) {
          return make_token(token_type::double_equal);
        } else if (match('>')) {
          return make_token(token_type::equal_right_arrow);
        } else {
          return make_token(token_type::equal);
        }
      case '<':
        return make_token(match('=') ? token_type::less_equal
                                     : token_type::less);
      case '>':
        return make_token(match('=') ? token_type::greater_equal
                                     : token_type::greator);

      // Literal tokens
      case '"':
        return string();
      default:
        return error_token("Unexpected character.");
      }
    }

    constexpr auto identifier() noexcept -> Token
    {
      while (isalpha(peek()) || (isdigit(peek()))) {
        advance();
      }

      return make_token(identifier_type());
    }

    constexpr auto number() noexcept -> Token
    {
      while (eml::isdigit(peek())) {
        advance();
      }

      // Look for a fractional part
      if (peek() == '.' && (std::isdigit(peek_next()) != 0)) {
        // Consume the "."
        advance();

        while (eml::isdigit(peek())) {
          advance();
        }
      }

      return make_token(token_type::number_literal);
    }

    constexpr auto string() noexcept -> Token
    {
      while (peek() != '"' && !at_end()) {
        if (peek() == '\n') {
          current_line++;
          current_line_start = (current + 2);
        }
        advance();
      }

      if (at_end()) {
        return error_token("Unterminated string.");
      }

      // Consume closing '"'
      advance();
      return make_token(token_type::string_literal);
    }

    constexpr auto at_end() const noexcept -> bool
    {
      return *current == '\0';
    }

    constexpr auto advance() noexcept -> char
    {
      ++current;
      return current[-1];
    }

    constexpr auto peek() const noexcept -> char
    {
      return *current;
    }

    constexpr auto peek_next() const noexcept -> char
    {
      EML_ASSERT(!at_end(), "call peek_next when at the end of a string");
      return current[1];
    }

    constexpr auto match(char expected) noexcept -> bool
    {
      if (*current != expected || at_end()) {
        return false;
      }

      current++;
      return true;
    }

    constexpr auto current_column() const noexcept -> std::size_t
    {
      return static_cast<std::size_t>(start - current_line_start + 1);
    }

    constexpr auto make_token(token_type type) const noexcept -> Token
    {
      return {type,
              {start, static_cast<std::size_t>(current - start)},
              FilePos{current_line, current_column()}};
    }

    constexpr auto error_token(const char* message) const noexcept -> Token
    {
      return {token_type::error, message,
              FilePos{current_line, current_column()}};
    }

    constexpr void skip_single_line_comment() noexcept
    {
      while (peek() != '\n' && !at_end()) {
        advance();
      }
    }

    constexpr void skip_whitespace() noexcept
    {
      while (true) {
        char c = peek();
        switch (c) {
        case ' ':
        case '\r':
        case '\t':
          advance();
          break;

        case '\n':
          current_line++;
          advance();
          current_line_start = current;
          break;

        case '/':
          if (peek_next() == '/') {
            skip_single_line_comment();
          } else {
            return;
          }
          break;

        default:
          return;
        }
      }
    }

    constexpr auto check_keyword(std::size_t start_offset,
                                 std::string_view rest, token_type type) const
        noexcept -> token_type
    {
      if (std::string_view{start + start_offset, rest.length()} == rest) {
        return type;
      }

      return token_type::identifier;
    }

    constexpr auto identifier_type() const noexcept -> token_type
    {
      switch (start[0]) {
      case 'a':
        if (current - start > 1) {
          switch (start[1]) {
          case 'n':
            return check_keyword(2, "d", token_type::keyword_and);
          case 's':
            return check_keyword(2, "ync", token_type::keyword_async);
          case 'w':
            return check_keyword(2, "ait", token_type::keyword_await);
          }
        }
        break;
      case 'c':
        if (current - start > 1) {
          switch (start[1]) {
          case 'a':
            return check_keyword(2, "se", token_type::keyword_case);
          case 'l':
            return check_keyword(2, "ass", token_type::keyword_class);
          }
        }
        break;
      case 'd':
        return check_keyword(1, "ef", token_type::keyword_def);
      case 'e':
        if (current - start > 1) {
          switch (start[1]) {
          case 'l':
            return check_keyword(2, "se", token_type::keyword_else);
          case 'x':
            return check_keyword(2, "tern", token_type::keyword_extern);
          }
        }
        break;
      case 'f':
        if (current - start > 1) {
          switch (start[1]) {
          case 'a':
            return check_keyword(2, "lse", token_type::keyword_false);
          case 'o':
            return check_keyword(2, "r", token_type::keyword_for);
          }
        }
        break;
      case 'i':
        return check_keyword(1, "f", token_type::keyword_if);
      case 'l':
        return check_keyword(1, "et", token_type::keyword_let);
      case 'n':
        return check_keyword(1, "ot", token_type::keyword_not);
      case 'o':
        return check_keyword(1, "r", token_type::keyword_or);
      case 'p':
        return check_keyword(1, "rint", token_type::keyword_print);
      case 'r':
        return check_keyword(1, "eturn", token_type::keyword_return);
      case 't':
        if (current - start > 1) {
          switch (start[1]) {
          case 'h':
            return check_keyword(2, "is", token_type::keyword_this);
          case 'r':
            return check_keyword(2, "ue", token_type::keyword_true);
          }
        }
        break;
      case 'u':
        return check_keyword(1, "nsafe", token_type::keyword_unsafe);
      case 'v':
        return check_keyword(1, "ariant", token_type::keyword_variant);
      }

      return token_type::identifier;
    }
  };

  constexpr auto begin() const -> iterator
  {
    return iterator{text};
  }
  constexpr auto end() const -> iterator
  {
    return iterator{"\0"};
  }
}; // namespace eml

} // namespace eml

#endif // EML_SCANNER_HPP
