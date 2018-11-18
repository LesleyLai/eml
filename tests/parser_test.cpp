#include "../src/scanner.hpp"
#include "../src/vm.hpp"

/**
 * @file parser.hpp
 * @brief A simple <a href="https://en.wikipedia.org/wiki/Pratt_parser">Pratt
 * Parser</a> for Embeded ML
 */

namespace eml {

chunk parse(std::string_view source);

class parser {
public:
  explicit parser(std::string_view source) : scanner_{source} {}

private:
  scanner scanner_;
};

chunk parse(std::string_view source)
{
  parser parser{source};
  return chunk{};
}

} // namespace eml

#include <catch2/catch.hpp>
