#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include <emscripten.h>

#include "eml.hpp"

static eml::GarbageCollector gc;
static eml::CompilerConfig config{eml::SameScopeShadowing::allow};
static eml::Compiler compiler(gc, config);
static eml::VM vm;

static std::string cache;

extern "C" {
EMSCRIPTEN_KEEPALIVE
const char* interpret(const char* line_c)
{
  std::string line{line_c};
  std::stringstream ss;
  if (!line.empty()) {
    compiler.compile(source)
        .map([&vm, &ss](auto tuple) {
          const auto [bytecode, type] = tuple;
          const auto result = vm.interpret(bytecode);
          if (result) {
            ss << eml::to_string(type, *result) << '\n';
          }
        })
        .map_error([&ss](const auto& errors) {
          std::for_each(std::begin(errors), std::end(errors),
                        [](auto e) { ss << eml::to_string(e) << '\n'; });
        });
  }

  cache = ss.str();
  return cache.c_str();
}
}

int main()
{
  // no-op
}
