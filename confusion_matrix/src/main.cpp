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

  auto res = cm::segment();

  (void)manualSegmentationPointsMap;
  (void)res;
}
