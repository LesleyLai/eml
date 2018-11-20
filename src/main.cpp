#include <iostream>
#include <string>

#include "parser.hpp"
#include "scanner.hpp"
#include "vm.hpp"

[[noreturn]] void repl()
{
  while (true) {
    std::cout << "> ";
    std::string line;
    std::getline(std::cin, line);
    if (!line.empty()) {
      const auto bytecode = eml::compile(line);
      if (bytecode) {
        // std::cout << *bytecode.disassemble() << '\n';
        eml::VM vm{*bytecode};
        std::cout << vm.interpret() << '\n';
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
