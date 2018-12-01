#ifndef EML_VM_HPP
#define EML_VM_HPP

#include <vector>

#include "ast.hpp"
#include "bytecode.hpp"

namespace eml {

class VM {
public:
  VM()
  {
    constexpr size_t initial_stack_size = 256;
    stack_.reserve(initial_stack_size);
  }

  /**
   * @brief Interpret the current code in the vm
   */
  [[nodiscard]] auto interpret(const Bytecode& code) -> std::optional<Value>;

private:
  std::vector<Value> stack_{}; // Stack of the vm
};

} // namespace eml

#endif // EML_VM_HPP
