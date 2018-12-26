#ifndef EML_MODULE_HPP
#define EML_MODULE_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include "type.hpp"
#include "value.hpp"

namespace eml {

/**
 * @brief The compiler policy about shadowing
 *
 * This runtime policy enum decides whether the compiler will silently allow
 * name shadowing, or provides a warning message
 */
enum class SameScopeShadowing {
  allow,   ///< @brief Allow shadowing in the same scope without any warning
  warning, ///< @brief Warn on shadowing in the same scope
};

using SymbolTable = std::unordered_map<std::string, std::pair<Type, Value>>;

/**
 * @brief A module and the top-level bindings it defined
 */
class Module {
public:
  explicit Module(std::string name) : name_{std::move(name)} {}

  void add_binding(const std::string& identifier, Type t, Value v)
  {
    //    auto query_result = global_env_.find(identifier);
    //    if (query_result != global_env_.end()) { // Shadowing

    //      //      if (options_.shadowing_policy ==
    //      SameScopeShadowing::warning) {
    //      //        std::clog << "Warning: Global value definition of " <<
    //      //        identifier
    //      //                  << " shadows earlier binding "
    //      //                     "in the global scope\n";
    //      //      }

    //      global_env_[identifier] = std::pair{std::move(t), std::move(v)};
    //    } else {
    //      global_env_.emplace(identifier, std::pair{std::move(t),
    //      std::move(v)});
    //    }

    global_env_.insert_or_assign(identifier,
                                 std::pair{std::move(t), std::move(v)});
  }

private:
  std::string name_;
  SymbolTable global_env_;
};

} // namespace eml

#endif // EML_MODULE_HPP
