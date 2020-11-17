#include <ostream>
#include <utility>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/algo/ranged_algorithms.hpp>

#include "distance_score.hpp"
#include "order_configurations_by_quality.hpp"

namespace cm {
ConfigWithDistanceScore::ConfigWithDistanceScore(
  Configuration p_config,
  std::uint64_t p_distScore)
  : config{std::move(p_config)}, distScore{p_distScore}
{
}

bool operator<(
  const ConfigWithDistanceScore& lhs,
  const ConfigWithDistanceScore& rhs) noexcept
{
  return lhs.distScore < rhs.distScore;
}

std::ostream& operator<<(
  std::ostream&                  os,
  const ConfigWithDistanceScore& configWithDistScore)
{
  return os << fmt::format(
           "ConfigWithDistanceScore{{\n"
           "\t\"config\": {},\n"
           "\t\"distScore\": {}\n"
           "}}",
           configWithDistScore.config,
           configWithDistScore.distScore);
}

[[nodiscard]] std::vector<ConfigWithDistanceScore> orderConfigurationsByQuality(
  const std::unordered_map<DataSetIdentifier, std::vector<std::uint64_t>>&
    manualSegmentationPoints,
  const std::unordered_map<
    Configuration,
    std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>&
    algorithmicallyDeterminedSegmentationPoints)
{
  std::vector<ConfigWithDistanceScore> result{};

  for (const auto& [config, map] :
       algorithmicallyDeterminedSegmentationPoints) {
    const std::uint64_t currentDistanceScore{
      distanceScore(map, manualSegmentationPoints)};
    result.emplace_back(config, currentDistanceScore);
  }

  pl::algo::sort(result);
  return result;
}
} // namespace cm
