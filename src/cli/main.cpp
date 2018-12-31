#include <algorithm>
#include <iostream>
#include <string>

#include "eml.hpp"

void repl()
{
  std::cout << "Embedded ML v" << eml::version::to_string() << '\n';

  std::pmr::unsynchronized_pool_resource mr;
  eml::GarbageCollector gc{mr};

  eml::CompilerConfig config = {eml::SameScopeShadowing::allow};
  eml::Compiler compiler{gc, config};
  eml::VM vm;

  while (true) {
    std::cout << "> ";

    const std::string source = []() {
      std::string line;
      std::string result;

      while (true) {
        std::getline(std::cin, line);
        if (!line.empty() && line.back() == '\\') {
          result += line.substr(0, line.size() - 1);
        } else {
          result += line;
          break;
        }
        std::cout << "... ";
      }
      return result;
    }();

    if (source == "exit()") {
      return;
    }

    if (!source.empty()) {
      compiler.compile(source)
          .map([&vm](auto tuple) {
            const auto [bytecode, type] = tuple;
            const auto result = vm.interpret(bytecode);
            if (result) {
              std::cout << eml::to_string(type, *result) << '\n';
            }
          })
          .map_error([](const auto& errors) {
            std::for_each(std::begin(errors), std::end(errors),
                          [](auto e) { std::clog << eml::to_string(e); });
          });
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
