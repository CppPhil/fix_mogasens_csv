#ifndef INCG_CM_ORDER_CONFIGURATIONS_BY_QUALITY_HPP
#define INCG_CM_ORDER_CONFIGURATIONS_BY_QUALITY_HPP
#include <cstdint>

#include <iosfwd>
#include <unordered_map>
#include <vector>

#include <cl/fs/path.hpp>

#include "configuration.hpp"
#include "data_set_identifier.hpp"

namespace cm {
struct ConfigWithDistanceScore {
  ConfigWithDistanceScore(Configuration p_config, std::uint64_t p_distScore);

  Configuration config;
  std::uint64_t distScore;
};

[[nodiscard]] bool operator<(
  const ConfigWithDistanceScore& lhs,
  const ConfigWithDistanceScore& rhs) noexcept;

std::ostream& operator<<(
  std::ostream&                  os,
  const ConfigWithDistanceScore& configWithDistScore);

[[nodiscard]] std::vector<ConfigWithDistanceScore> orderConfigurationsByQuality(
  const std::unordered_map<DataSetIdentifier, std::vector<std::uint64_t>>&
    manualSegmentationPoints,
  const std::unordered_map<
    Configuration,
    std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>&
    algorithmicallyDeterminedSegmentationPoints);
} // namespace cm
#endif // INCG_CM_ORDER_CONFIGURATIONS_BY_QUALITY_HPP
