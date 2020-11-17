#ifndef INCG_CM_CLOSEST_ONE_HPP
#define INCG_CM_CLOSEST_ONE_HPP
#include <cstdint>

#include <vector>

namespace cm {
[[nodiscard]] std::uint64_t closestOne(
  std::uint64_t                     algorithmicallyDeterminedSegmentationPoint,
  const std::vector<std::uint64_t>& manualSegmentationPoints);
} // namespace cm
#endif // INCG_CM_CLOSEST_ONE_HPP
