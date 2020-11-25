#ifndef INCG_CM_CONFUSION_MATRIX_BEST_CONFIGS_HPP
#define INCG_CM_CONFUSION_MATRIX_BEST_CONFIGS_HPP
#include <functional>
#include <iosfwd>
#include <unordered_map>
#include <vector>

#include "configuration.hpp"
#include "confusion_matrix.hpp"
#include "data_set_identifier.hpp"

namespace cm {
/*!
 * \brief A `Configuration` with a `ConfusionMatrix`.
 **/
struct ConfigWithTotalConfusionMatrix {
  /*!
   * \brief Prints a `ConfigWithTotalConfusionMatrix` to `os`.
   * \param os The ostream to print to.
   * \param obj The `ConfigWithTotalConfusionMatrix` to print.
   * \return `os`
   **/
  friend std::ostream& operator<<(
    std::ostream&                         os,
    const ConfigWithTotalConfusionMatrix& obj);

  /*!
   * \brief Default constructor
   **/
  ConfigWithTotalConfusionMatrix() = default;

  /*!
   * \brief Constructor.
   * \param p_config The `Configuration` to use.
   * \param p_matrix The `ConfusionMatrix` to use.
   **/
  ConfigWithTotalConfusionMatrix(
    Configuration   p_config,
    ConfusionMatrix p_matrix);

  Configuration   config; /*!< The `Configuration` */
  ConfusionMatrix matrix; /*!< The associated `ConfusionMatrix` */
};

/*!
 * \def CM_SORTER
 * \brief Macro to define a sorter based on a single criterion for
 *        `ConfigWithTotalConfusionMatrix` objects.
 **/
#define CM_SORTER(criterion, op)                                \
  inline constexpr struct {                                     \
    [[nodiscard]] bool operator()(                              \
      const ConfigWithTotalConfusionMatrix& lhs,                \
      const ConfigWithTotalConfusionMatrix& rhs) const noexcept \
    {                                                           \
      return lhs.matrix.criterion() op rhs.matrix.criterion();  \
    }                                                           \
  } criterion##Sorter

/*!
 * \brief Sorts `ConfigWithTotalConfusionMatrix` objects by true positives
 *        (highest first)
 **/
CM_SORTER(truePositives, >);

/*!
 * \brief Sorts `ConfigWithTotalConfusionMatrix` objects by true negatives
 *        (highest first)
 **/
CM_SORTER(trueNegatives, >);

/*!
 * \brief Sorts `ConfigWithTotalConfusionMatrix` objects by false positives
 *        (lowest first)
 **/
CM_SORTER(falsePositives, <);

/*!
 * \brief Sorts `ConfigWithTotalConfusionMatrix` objects by false negatives
 *        (lowest first)
 **/
CM_SORTER(falseNegatives, <);

/*!
 * \brief Sorter to sort `ConfigWithTotalConfusionMatrix` objects by the
 *        count of true positives minus the count of false positives
 *        minus the count of false negatives.
 **/
inline constexpr struct {
  [[nodiscard]] bool operator()(
    const ConfigWithTotalConfusionMatrix& lhs,
    const ConfigWithTotalConfusionMatrix& rhs) const noexcept
  {
    const std::uint64_t lhsValue{
      lhs.matrix.truePositives() - lhs.matrix.falsePositives()
      - lhs.matrix.falseNegatives()};
    const std::uint64_t rhsValue{
      rhs.matrix.truePositives() - rhs.matrix.falsePositives()
      - rhs.matrix.falseNegatives()};

    return lhsValue < rhsValue;
  }
} disregardTrueNegativesSorter;

/*!
 * \brief Sorter to sort `ConfigWithTotalConfusionMatrix` objects by the
 *        sum of true positives and true negatives minus the false positives
 *        minus the false negatives.
 **/
inline constexpr struct {
  [[nodiscard]] bool operator()(
    const ConfigWithTotalConfusionMatrix& lhs,
    const ConfigWithTotalConfusionMatrix& rhs) const noexcept
  {
    const std::uint64_t lhsValue{
      lhs.matrix.truePositives() + lhs.matrix.trueNegatives()
      - lhs.matrix.falsePositives() - lhs.matrix.falseNegatives()};
    const std::uint64_t rhsValue{
      rhs.matrix.truePositives() + rhs.matrix.trueNegatives()
      - rhs.matrix.falsePositives() - rhs.matrix.falseNegatives()};

    return lhsValue < rhsValue;
  }
} addTrueSubtractFalseSorter;

/*!
 * \brief Determines the 'best' configurations.
 * \param manualSegmentationPoints The manual segmentation points (ground
 *                                 truth).
 * \param algorithmicallyDeterminedSegmentationPoints The segmentation
 *                                                    points found by the Python
 *                                                    application.
 * \param sorter The sorter to use.
 * \return A vector of `ConfigWithTotalConfusionMatrix` objects sorted by
 *         `sorter`.
 * \throws cl::Exception if `sorter` does not contain a valid target.
 **/
[[nodiscard]] std::vector<ConfigWithTotalConfusionMatrix>
confusionMatrixBestConfigs(
  const std::unordered_map<DataSetIdentifier, std::vector<std::uint64_t>>&
    manualSegmentationPoints,
  const std::unordered_map<
    Configuration,
    std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>&
                                             algorithmicallyDeterminedSegmentationPoints,
  const std::function<bool(
    const ConfigWithTotalConfusionMatrix&,
    const ConfigWithTotalConfusionMatrix&)>& sorter);
} // namespace cm
#endif // INCG_CM_CONFUSION_MATRIX_BEST_CONFIGS_HPP
