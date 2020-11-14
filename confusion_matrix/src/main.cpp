#include <cstdlib>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "manual_segmentation_point.hpp"
#include "segment.hpp"

int main()
{
  // TODO: HERE
  // TODO: This is just experimental.
  const std::unordered_map<
    cm::DataSetIdentifier,
    std::vector<cm::ManualSegmentationPoint>>
    manualSegmentationPointsMap{cm::ManualSegmentationPoint::readCsvFile()};

  const std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>> res
    = cm::segment();

  (void)manualSegmentationPointsMap;
  (void)res;

  for (const auto& [path, segmentationPoints] : res) {
    fmt::print("\"{}\": \"{}\"\n", path, fmt::join(segmentationPoints, ", "));
  }

  fmt::print("DONE.\n");
  return EXIT_SUCCESS;
}
