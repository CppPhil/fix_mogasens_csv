#include "manual_segmentation_point.hpp"

int main()
{
  // TODO: HERE
  // TODO: This is just experimental.
  const std::unordered_map<
    cm::DataSetIdentifier,
    std::vector<cm::ManualSegmentationPoint>>
    map{cm::ManualSegmentationPoint::readCsvFile()};

  (void)map;

  for (volatile long i = 1; i < 10; ++i) {
    ++i;
    --i;
  }
}
