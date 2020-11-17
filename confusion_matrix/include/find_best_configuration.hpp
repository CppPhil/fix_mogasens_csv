#ifndef INCG_CM_FIND_BEST_CONFIGURATION_HPP
#define INCG_CM_FIND_BEST_CONFIGURATION_HPP
#include <cstdint>

#include <unordered_map>
#include <vector>

#include <cl/fs/path.hpp>

#include "configuration.hpp"
#include "data_set_identifier.hpp"

namespace cm {
[[nodiscard]] Configuration findBestConfiguration(
  const std::unordered_map<DataSetIdentifier, std::vector<std::uint64_t>>&
    manualSegmentationPoints,
  const std::unordered_map<
    Configuration,
    std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>&
                 algorithmicallyDeterminedSegmentationPoints,
  std::uint64_t* distScore = nullptr);
} // namespace cm
#endif // INCG_CM_FIND_BEST_CONFIGURATION_HPP
