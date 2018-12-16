#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include <emscripten.h>

#include "eml.hpp"
#include "vm.hpp"

static eml::CompilerConfig config{eml::Shadowing::allow};
static eml::Compiler compiler(config);
static eml::VM vm;

static std::string cache;

extern "C" {
EMSCRIPTEN_KEEPALIVE
const char* interpret(const char* line_c) {
    std::string line {line_c};
    std::stringstream ss;
    if (!line.empty()) {

        auto bytecode = compiler.compile(line);
        if (bytecode) {
            const auto result = vm.interpret(*bytecode);
            if (result) {
                ss << *result << '\n';
            }
        } else {
            const auto& errors = bytecode.error();
            std::for_each(std::begin(errors), std::end(errors),
                          [&ss](auto e) {
                ss << eml::to_string(e) << '\n';
            });
        }
    }

    cache = ss.str();
    return cache.c_str();
}
}

int main() {
  // no-op
}
