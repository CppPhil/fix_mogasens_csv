#include <tl/optional.hpp>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <cl/exception.hpp>

#include "distance_score.hpp"
#include "find_best_configuration.hpp"

namespace cm {
Configuration findBestConfiguration(
  const std::unordered_map<DataSetIdentifier, std::vector<std::uint64_t>>&
    manualSegmentationPoints,
  const std::unordered_map<
    Configuration,
    std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>&
                 algorithmicallyDeterminedSegmentationPoints,
  std::uint64_t* distScore)
{
  std::uint64_t               lowestDistanceScore{UINT64_MAX};
  tl::optional<Configuration> bestConfiguration{tl::nullopt};

  for (const auto& [config, map] :
       algorithmicallyDeterminedSegmentationPoints) {
    const std::uint64_t currentDistanceScore{
      distanceScore(map, manualSegmentationPoints)};

    // TODO: HERE: DEBUG
    fmt::print("{} has distance score {}\n\n", config, currentDistanceScore);
    // TODO: HERE: DEBUG

    if (currentDistanceScore < lowestDistanceScore) {
      lowestDistanceScore = currentDistanceScore;
      bestConfiguration   = config;
    }
  }

  if (!bestConfiguration.has_value()) {
    CL_THROW("No best configuration found!");
  }

  if (distScore != nullptr) { *distScore = lowestDistanceScore; }

  return *bestConfiguration;
}
} // namespace cm
