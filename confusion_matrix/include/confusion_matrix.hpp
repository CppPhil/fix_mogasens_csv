#ifndef INCG_CM_CONFUSION_MATRIX_HPP
#define INCG_CM_CONFUSION_MATRIX_HPP
#include <cstdint>

namespace cm {
// TODO:
// TODO: Iterate over all the valid hardware timestamps for each Path thingie
//       and check if the current timestamp is a true positive, true negative
//       false positive or false negative
class ConfusionMatrix {
public:
  using this_type = ConfusionMatrix;

  ConfusionMatrix();

  [[nodiscard]] std::uint64_t truePositives() const noexcept;

  [[nodiscard]] std::uint64_t trueNegatives() const noexcept;

  [[nodiscard]] std::uint64_t falsePositives() const noexcept;

  [[nodiscard]] std::uint64_t falseNegatives() const noexcept;

  [[nodiscard]] std::uint64_t totalCount() const noexcept;

  this_type& incrementTruePositives() noexcept;

  this_type& incrementTrueNegatives() noexcept;

  this_type& incrementFalsePositives() noexcept;

  this_type& incrementFalseNegatives() noexcept;

  this_type& operator+=(const ConfusionMatrix& other) noexcept;

private:
  // If a given fake one exists within the range of a real one.
  std::uint64_t m_truePositives;

  // If there's no real one and no fake one for that as well.
  std::uint64_t m_trueNegatives;

  // If a given fake one is not within the range of any real one.
  std::uint64_t m_falsePositives;

  // If for a given real one there is no fake one to be found in its range.
  std::uint64_t m_falseNegatives;

  std::uint64_t m_totalCount;
};
} // namespace cm
#endif // INCG_CM_CONFUSION_MATRIX_HPP
