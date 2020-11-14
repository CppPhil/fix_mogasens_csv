#ifndef INCG_CM_SEGMENT_HPP
#define INCG_CM_SEGMENT_HPP
#include <cstdint>

#include <unordered_map>
#include <vector>

#include <cl/fs/path.hpp>

namespace cm {
/*!
 * Invokes Python to segment the interpolated data sets.
 * \return A map that maps the paths to the interpolated data sets to vectors
 *         of the hardware timestamps (in milliseconds) that are segmentation
 *         points.
 * \throws cl::Exception if an error occurs.
 **/
[[nodiscard]] std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>
segment();
} // namespace cm
#endif // INCG_CM_SEGMENT_HPP
