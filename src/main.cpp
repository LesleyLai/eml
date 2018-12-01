#include <algorithm>
#include <iostream>
#include <string>

#include "eml.hpp"

[[noreturn]] void repl()
{
  std::cout << "Embedded ML v" << eml::version::to_string() << '\n';

  eml::CompilerConfig config{eml::Shadowing::allow};
  eml::Compiler compiler(config);
  eml::VM vm;

  while (true) {
    std::cout << "> ";
    std::string line;
    std::getline(std::cin, line);

    if (!line.empty()) {
      auto bytecode = compiler.compile(line);
      if (bytecode) {
        const auto result = vm.interpret(*bytecode);
        if (result) {
          std::cout << *result << '\n';
        }
      } else {
        const auto& errors = bytecode.error();
        std::for_each(std::begin(errors), std::end(errors),
                      [](auto e) { std::clog << eml::to_string(e) << '\n'; });
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
