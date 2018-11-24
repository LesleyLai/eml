#include <iostream>
#include <sstream>

#include "error.hpp"

namespace eml {

// void report(const CompilationError& error)
//{
//  std::clog << "L" << error.pos_.line << ':' << error.pos_.column;

//  switch (error.type_) {
//  case CompilationError::type::syntax_error:
//    std::clog << " Syntax Error: ";
//    break;

//  case CompilationError::type::type_error:
//    return;
//  }

//  std::clog << error.msg_ << '\n';
//}

struct ErrorPrinter {
  std::ostream& os_;

  explicit ErrorPrinter(std::ostream& os) : os_{os} {}
  void operator()(const SyntaxError& e)
  {
    os_ << "L" << e.at.position.line << ':' << e.at.position.column;
    os_ << " Syntax Error";

    switch (e.at.type) {
    case token_type::eof:
      os_ << " at end";
      break;
    case token_type::error:
      break;
    default:
      os_ << " at \"" << e.at.text << '\"';
    }

    os_ << ": " << e.msg;
  }
};

std::string to_string(const CompilationError& error)
{
  std::stringstream ss;
  std::visit(ErrorPrinter{ss}, error);
  return ss.str();
}

} // namespace eml
