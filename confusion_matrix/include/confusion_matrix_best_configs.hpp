#ifndef INCG_CM_CONFUSION_MATRIX_BEST_CONFIGS_HPP
#define INCG_CM_CONFUSION_MATRIX_BEST_CONFIGS_HPP
#include <iosfwd>
#include <unordered_map>
#include <vector>

#include "configuration.hpp"
#include "confusion_matrix.hpp"
#include "data_set_identifier.hpp"

namespace cm {
struct ConfigWithTotalConfusionMatrix {
  ConfigWithTotalConfusionMatrix(
    Configuration   p_config,
    ConfusionMatrix p_matrix);

  Configuration   config;
  ConfusionMatrix matrix;
};

[[nodiscard]] bool operator<(
  const ConfigWithTotalConfusionMatrix& lhs,
  const ConfigWithTotalConfusionMatrix& rhs) noexcept;

std::ostream& operator<<(
  std::ostream&                         os,
  const ConfigWithTotalConfusionMatrix& obj);

[[nodiscard]] std::vector<ConfigWithTotalConfusionMatrix>
confusionMatrixBestConfigs(
  const std::unordered_map<DataSetIdentifier, std::vector<std::uint64_t>>&
    manualSegmentationPoints,
  const std::unordered_map<
    Configuration,
    std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>&
    algorithmicallyDeterminedSegmentationPoints);
} // namespace cm
#endif // INCG_CM_CONFUSION_MATRIX_BEST_CONFIGS_HPP
