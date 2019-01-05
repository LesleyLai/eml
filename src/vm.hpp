#ifndef EML_VM_HPP
#define EML_VM_HPP

#include <vector>

#include "ast.hpp"
#include "bytecode.hpp"
#include "memory.hpp"

namespace eml {

class VM {
public:
  explicit VM(GarbageCollector& gc) noexcept : garbage_collector_{gc}
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
  std::reference_wrapper<GarbageCollector> garbage_collector_;
};

} // namespace eml

#endif // EML_VM_HPP
