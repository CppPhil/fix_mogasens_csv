#ifndef INCG_CM_CREATE_SEGMENTATION_RESULTS_HPP
#define INCG_CM_CREATE_SEGMENTATION_RESULTS_HPP
#include <cstdint>

#include <unordered_map>
#include <vector>

#include <cl/fs/path.hpp>

#include "configuration.hpp"

namespace cm {
std::unordered_map<
  cm::Configuration,
  std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>
createSegmentationResults();
} // namespace cm
#endif // INCG_CM_CREATE_SEGMENTATION_RESULTS_HPP
