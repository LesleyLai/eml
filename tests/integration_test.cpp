#include <ApprovalTests.hpp>
#include <catch2/catch.hpp>

#include "debug.hpp"
#include "eml.hpp"
#include "eml_disassembler.hpp"

#include "utils/approval_using_sections_namer.hpp"

namespace {
enum class ForDocumentation { no, yes };

void verify_eml(const std::string_view source,
                ForDocumentation for_documentation = ForDocumentation::no)
{
  eml::GarbageCollector gc{};

  eml::CompilerConfig config = {eml::SameScopeShadowing::allow};
  eml::Compiler compiler{gc, config};
  eml::VM vm{gc};

  std::stringstream ss;
  if (for_documentation == ForDocumentation::no) {
    ss << "Input:\n" << source << "\n\n";
  }

  if (!source.empty()) {
    eml::parse(source, gc)
        .and_then([&](std::unique_ptr<eml::AstNode>&& ast) {
          if (for_documentation == ForDocumentation::no) {
            ss << "Parse into:\n" << eml::to_string(*ast) << "\n\n";
          }
          return compiler.type_check(ast);
        })
        .map([&](auto ast) { return compiler.generate_code(*ast); })
        .map([&](auto tuple) {
          const auto& [bytecode, type] = tuple;

          if (for_documentation == ForDocumentation::no) {
            ss << "Compile into:\n" << disassemble(bytecode) << '\n';
          }

          const auto result = vm.interpret(bytecode);
          REQUIRE(result);
          if (for_documentation == ForDocumentation::no) {
            ss << "Executes to:\n";
          }
          ss << eml::to_string(type, *result);
        })
        .map_error([&ss](const auto& errors) {
          ss << "Error message:\n";
          std::for_each(std::begin(errors), std::end(errors),
                        [&ss](auto e) { ss << eml::to_string(e); });
        });

    const auto options = [&]() {
      if (for_documentation == ForDocumentation::yes) {
        return ApprovalTests::Options{}.fileOptions().withFileExtension(
            ".docs.txt");
      }
      return ApprovalTests::Options{};
    }();

    ApprovalTests::Approvals::verify(ss.str(), options);
  }
}

} // anonymous namespace

TEST_CASE("If", "[.][integration]")
{
  auto directoryDisposer =
      ApprovalTests::Approvals::useApprovalsSubdirectory("approval_tests/if");
  auto default_namer_disposer =
      ApprovalTestsPlus::BySectionsNamer::useAsDefaultNamer();

  SECTION("If Else")
  {
    constexpr std::string_view s = "if (5 > 1) {2 + 3} else {4 - 6}";
    verify_eml(s);
  }

  SECTION("Else If")
  {
    auto filename =
        ApprovalTests::Approvals::getDefaultNamer()->getApprovedFile(".eml");
    auto source = ApprovalTests::FileUtils::readFileThrowIfMissing(filename);
    verify_eml(source);
    verify_eml(source, ForDocumentation::yes);
  }
}

struct LanguageTest {
  const char* name = nullptr;
  const std::string_view src;
};

TEST_CASE("Test Syntax Errors", "[.][integration]")
{
  auto directoryDisposer = ApprovalTests::Approvals::useApprovalsSubdirectory(
      "approval_tests/syntax_errors");
  auto default_namer_disposer =
      ApprovalTestsPlus::BySectionsNamer::useAsDefaultNamer();

  static constexpr LanguageTest tests[] = {
      {"missing )", "if (5 > 1 {2 + 3} else {4 - 6}"},
      {"missing }", "if (5 > 1) {2 + 3 else {4 - 6}"},
  };

  for (const auto& [name, src] : tests) {
    SECTION(name)
    {
      verify_eml(src);
    }
  }
}

TEST_CASE("Test Type Errors", "[.][integration]")
{
  auto directoryDisposer = ApprovalTests::Approvals::useApprovalsSubdirectory(
      "approval_tests/type_errors");
  auto default_namer_disposer =
      ApprovalTestsPlus::BySectionsNamer::useAsDefaultNamer();

  static constexpr LanguageTest tests[] = {
      {"Number in branch", "if (1) 2 else 3"},
      {"Branch type mismatch", "if (true) 3 else false"},
      {"Boolean in arithmatics", "1 + false"},
      {"Number in boolean expression", "!1"}};

  for (const auto& [name, src] : tests) {
    SECTION(name)
    {
      verify_eml(src);
    }
  }
}

// TEST_CASE("Test with String")
//{
//  const std::string s = R"("Hello, " ++ "world")";
//  verify_eml(s);
//}
