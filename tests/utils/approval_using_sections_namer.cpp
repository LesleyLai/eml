#include "approval_using_sections_namer.hpp"

#include "catch2/catch.hpp"
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>

namespace ApprovalTestsPlus {

auto BySectionsNamer::getFullFileNameWithExtraDirectory(
    const std::string& approved, const std::string& extensionWithDot) const
    -> std::string
{
  std::string outputDirectory = getDirectory();
  std::string outputFile =
      getSectionNames() + "." + approved + extensionWithDot;
  return outputDirectory + outputFile;
}

auto BySectionsNamer::getApprovedFile(std::string extensionWithDot) const
    -> std::string
{
  return getFullFileNameWithExtraDirectory("approved", extensionWithDot);
}

auto BySectionsNamer::getReceivedFile(std::string extensionWithDot) const
    -> std::string
{
  return getFullFileNameWithExtraDirectory("received", extensionWithDot);
}

auto BySectionsNamer::useAsDefaultNamer() -> ApprovalTests::DefaultNamerDisposer
{
  return ApprovalTests::Approvals::useAsDefaultNamer(
      []() { return std::make_shared<BySectionsNamer>(); });
}

auto BySectionsNamer::getSectionNames() -> std::string
{
  // Horrible copy-and-paste from ApprovalTestNamer::getTestName()
  // Differing only by ignoring the first element of getCurrentTest().sections,
  // which contains the name of the current test...
  std::stringstream ext;
  auto test = getCurrentTest();
  if (test.sections.size() <= 1) {
    throw std::domain_error(
        "ApprovalOnlyUsingSectionsNamer only supports tests inside SECTIONs");
  }
  size_t firstSection = 1; // 0 contains the test name
  for (size_t i = firstSection; i < test.sections.size(); i++) {
    if (firstSection < i) {
      ext << ".";
    }
    ext << test.sections[i];
  }

  return convertToFileName(ext.str());
}

} // namespace ApprovalTestsPlus

using Catch::Matchers::EndsWith;

TEST_CASE("BySectionsNamer ignores File Name and Test Name")
{
  auto default_namer_disposer =
      ApprovalTestsPlus::BySectionsNamer::useAsDefaultNamer();

  auto namer = ApprovalTests::Approvals::getDefaultNamer();
  SECTION("if_else")
  {
    auto sep = ApprovalTests::SystemUtils::getDirectorySeparator();
    REQUIRE_THAT(namer->getApprovedFile(".txt"),
                 EndsWith(sep + "if_else.approved.txt"));
    REQUIRE_THAT(namer->getReceivedFile(".txt"),
                 EndsWith(sep + "if_else.received.txt"));
  }
  CHECK_THROWS(namer->getApprovedFile(".txt"));
  CHECK_THROWS(namer->getReceivedFile(".txt"));
}

TEST_CASE(
    "BySectionsNamer Ignoring File Name and Test Name and tags are ignored",
    "[IAmATag]")
{
  auto default_namer_disposer =
      ApprovalTestsPlus::BySectionsNamer::useAsDefaultNamer();
  auto namer = ApprovalTests::Approvals::getDefaultNamer();
  SECTION("if_else")
  {
    auto sep = ApprovalTests::SystemUtils::getDirectorySeparator();
    REQUIRE_THAT(namer->getApprovedFile(".txt"),
                 EndsWith(sep + "if_else.approved.txt"));
    REQUIRE_THAT(namer->getReceivedFile(".txt"),
                 EndsWith(sep + "if_else.received.txt"));
  }
  CHECK_THROWS(namer->getApprovedFile(".txt"));
  CHECK_THROWS(namer->getReceivedFile(".txt"));
}