#ifndef EML_TEST_APPROVAL_USING_SECTIONS_NAMER_HPP
#define EML_TEST_APPROVAL_USING_SECTIONS_NAMER_HPP

#include <ApprovalTests.hpp>
#include <string>

namespace ApprovalTestsPlus {

class BySectionsNamer : public ApprovalTests::ApprovalTestNamer {
public:
  [[nodiscard]] auto
  getFullFileNameWithExtraDirectory(const std::string& approved,
                                    const std::string& extensionWithDot) const
      -> std::string;

  [[nodiscard]] auto getApprovedFile(std::string extensionWithDot) const
      -> std::string override;

  [[nodiscard]] auto getReceivedFile(std::string extensionWithDot) const
      -> std::string override;

  static auto useAsDefaultNamer() -> ApprovalTests::DefaultNamerDisposer;

private:
  [[nodiscard]] static auto getSectionNames() -> std::string;
};

} // namespace ApprovalTestsPlus

#endif // EML_TEST_APPROVAL_USING_SECTIONS_NAMER_HPP
