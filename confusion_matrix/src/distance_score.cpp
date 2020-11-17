#include "distance_score.hpp"
#include "closest_one.hpp"
#include "distance.hpp"
#include "fetch.hpp"

namespace cm {
std::uint64_t distanceScore(
  const std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>&
    segmentationPointsForConfig,
  const std::unordered_map<DataSetIdentifier, std::vector<std::uint64_t>>&
    manualSegmentationPoints)
{
  std::uint64_t totalDistanceScore{0};

  for (const auto& [path, segmentationPoints] : segmentationPointsForConfig) {
    const DataSetIdentifier           dsi{toDataSetIdentifier(path)};
    const std::vector<std::uint64_t>& manualOnes{
      fetch(manualSegmentationPoints, dsi)};

    std::uint64_t pathScore{0};

    for (const std::uint64_t segmentationPoint : segmentationPoints) {
      const std::uint64_t closestManualOne{
        closestOne(segmentationPoint, manualOnes)};
      const std::uint64_t dist{distance(segmentationPoint, closestManualOne)};
      pathScore += dist;
    }

    totalDistanceScore += pathScore;
  }

  return totalDistanceScore;
}
} // namespace cm
