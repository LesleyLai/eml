#include <iomanip>
#include <iostream>
#include <sstream>
#include <variant>
#include <vector>

namespace eml {

using Value = std::variant<double>;

enum class opcode : std::uint8_t {
  ret = 0,
};

class Chunk {
public:
  void write(opcode code) { codes_.push_back(code); }

  auto disassemble() const -> std::string;

private:
  std::vector<opcode> codes_;
};

std::string Chunk::disassemble() const
{
  std::stringstream ss;

  for (auto i = size_t{0}; i < codes_.size(); ++i) {
    switch (codes_[i]) {
    case opcode::ret:
      ss << std::hex << std::setfill('0') << std::setw(2)
         << static_cast<unsigned int>(codes_[i]);
      ss << std::setfill(' ') << std::setw(16) << "return\n";
      break;
    }
  }

  return ss.str();
}

} // namespace eml

int main()
{
  using namespace eml;
  Chunk chunk;
  chunk.write(opcode::ret);
  chunk.write(opcode::ret);
  chunk.write(opcode::ret);
  std::cout << chunk.disassemble();
  return 0;
}
