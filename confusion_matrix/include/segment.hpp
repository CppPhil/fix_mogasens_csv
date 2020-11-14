#ifndef INCG_CM_SEGMENT_HPP
#define INCG_CM_SEGMENT_HPP
#include <cstdint>

#include <unordered_map>
#include <vector>

#include "data_set_identifier.hpp"

namespace cm {
/*!
 *
 * \return
 * \throws cl::Exception if the interpolated data set paths could not be
 *                       fetched.
 **/
[[nodiscard]] std::unordered_map<DataSetIdentifier, std::vector<std::uint64_t>>
segment();
} // namespace cm
#endif // INCG_CM_SEGMENT_HPP
