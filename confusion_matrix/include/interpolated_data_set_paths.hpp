#ifndef INCG_CM_INTERPOLATED_DATA_SET_PATHS_HPP
#define INCG_CM_INTERPOLATED_DATA_SET_PATHS_HPP
#include <vector>

#include <cl/fs/path.hpp>

namespace cm {
/*!
 * \brief Returns the paths to the interpolated data sets.
 * \return The interpolated data set paths.
 * \throws cl::Exception on error.
 **/
[[nodiscard]] std::vector<cl::fs::Path> interpolatedDataSetPaths();
} // namespace cm
#endif // INCG_CM_INTERPOLATED_DATA_SET_PATHS_HPP
