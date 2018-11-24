#include <iostream>
#include <string>

#include "compiler.hpp"
#include "debug.hpp"
#include "parser.hpp"
#include "scanner.hpp"
#include "vm.hpp"

[[noreturn]] void repl() {
  std::cout << "Embedded ML v" << eml::version::to_string() << '\n';

  while (true) {
    std::cout << "> ";
    std::string line;
    std::getline(std::cin, line);
    if (!line.empty()) {
      const auto ast = eml::parse(line);
      if (ast) {
        std::cout << eml::string_from_ast(**ast) << "\n\n";
        const auto bytecode = eml::bytecode_from_ast(**ast);
        eml::VM vm{bytecode};
        std::cout << vm.interpret() << '\n';
      } else {
        for (const auto& e : ast.error()) {
          std::clog << e.msg_ << '\n';
        }
      }
    }
  }
}

int main(int argc, char** /*argv*/)
{
  if (argc < 2) {
    repl();
  } else {
    std::cout << "Read file!\n";
  }

  return 0;
}
