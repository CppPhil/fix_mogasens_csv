#ifndef INCG_CM_CLOSEST_ONE_HPP
#define INCG_CM_CLOSEST_ONE_HPP
#include <cstdint>

#include <vector>

namespace cm {
/*!
 * \brief Finds the segmentation point in `manualSegmentationPoints` that is the
 *        closest to `algorithmicallyDeterminedSegmentationPoint`.
 * \param algorithmicallyDeterminedSegmentationPoint The segmentation point to
 *                                                   find the closest one to.
 * \param manualSegmentationPoints The manual segmentation points.
 * \return The manual segmentation point that the the
 *         closest to `algorithmicallyDeterminedSegmentationPoint`.
 * \throws cl::Exception if no segmentation point was found.
 **/
[[nodiscard]] std::uint64_t closestOne(
  std::uint64_t                     algorithmicallyDeterminedSegmentationPoint,
  const std::vector<std::uint64_t>& manualSegmentationPoints);
} // namespace cm
#endif // INCG_CM_CLOSEST_ONE_HPP
