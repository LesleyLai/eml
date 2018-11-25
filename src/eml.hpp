#ifndef EML_HPP
#define EML_HPP

/**
 * @dir src All the source files of eml
 */

/**
 * @file eml.hpp
 * @brief This file provides the public api of EML
 */

#include "common.hpp"
#include "error.hpp"
#include "expected.hpp"
#include "vm.hpp"

/**
 * @namespace eml
 * @brief The Embedded ML Library
 */
namespace eml {

using CompileResult = expected<chunk, std::vector<CompilationError>>;

/**
 * @brief compiles the source into bytecode
 *
 * @return A bytecode chunk if the compilation process succeed, a vector of
 * errors otherwise
 */
auto compile(std::string_view src) -> CompileResult;

} // namespace eml

#endif // EML_HPP
