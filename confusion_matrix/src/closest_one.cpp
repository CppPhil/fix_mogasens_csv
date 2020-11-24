#include <algorithm>

#include <tl/optional.hpp>

#include <cl/exception.hpp>

#include "closest_one.hpp"
#include "distance.hpp"

namespace cm {
std::uint64_t closestOne(
  std::uint64_t                     algorithmicallyDeterminedSegmentationPoint,
  const std::vector<std::uint64_t>& manualSegmentationPoints)
{
  std::uint64_t               smallestDistance{UINT64_MAX};
  tl::optional<std::uint64_t> closest{tl::nullopt};

  for (const std::uint64_t manualSegmentationPoint : manualSegmentationPoints) {
    const std::uint64_t dist{distance(
      algorithmicallyDeterminedSegmentationPoint, manualSegmentationPoint)};

    if (dist < smallestDistance) {
      smallestDistance = dist;
      closest          = manualSegmentationPoint;
    }
  }

  if (!closest.has_value()) { CL_THROW("Could not find closest value!"); }

  return *closest;
}
} // namespace cm
