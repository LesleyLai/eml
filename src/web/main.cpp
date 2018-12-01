#include <algorithm>
#include <iostream>
#include <string>

#include <emscripten.h>

#include "eml.hpp"
#include "vm.hpp"

eml::CompilerConfig config{eml::Shadowing::allow};
eml::Compiler compiler(config);
eml::VM vm;

int y = 0;

extern "C" {
EMSCRIPTEN_KEEPALIVE
void interpret(const char* line_c) {
  std::string line {line_c};
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
                      [](auto e) {
                        std::clog << eml::to_string(e) << '\n';
                      });
      }
    }
  }
}

int main() {
    std::cout << "Embedded ML v" << eml::version::to_string() << '\n';
}
