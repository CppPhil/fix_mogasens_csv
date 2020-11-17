#ifndef INCG_CM_DISTANCE_SCORE_HPP
#define INCG_CM_DISTANCE_SCORE_HPP
#include <cstdint>

#include <unordered_map>
#include <vector>

#include <cl/fs/path.hpp>

#include "data_set_identifier.hpp"

namespace cm {
[[nodiscard]] std::uint64_t distanceScore(
  const std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>&
    segmentationPointsForConfig,
  const std::unordered_map<DataSetIdentifier, std::vector<std::uint64_t>>&
    manualSegmentationPoints);
} // namespace cm
#endif // INCG_CM_DISTANCE_SCORE_HPP
