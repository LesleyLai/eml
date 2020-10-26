#ifndef EML_DISASSEMBLER_HPP
#define EML_DISASSEMBLER_HPP

#include <string>

namespace eml {

struct Bytecode;

[[nodiscard]] auto disassemble(const Bytecode& bytecode) -> std::string;

} // namespace eml

#endif // EML_DISASSEMBLER_HPP
