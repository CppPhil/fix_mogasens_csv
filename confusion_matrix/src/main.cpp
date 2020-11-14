#include <cstdlib>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "manual_segmentation_point.hpp"
#include "segment.hpp"

int main()
{
  try { // TODO: HERE
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
  }
  catch (const cl::Exception& ex) {
    fmt::print(stderr, "{}: caught cl::Exception\n", PL_CURRENT_FUNCTION);
    fmt::print(
      stderr,
      "Thrown at file: \"{}\", line: {}, function: \"{}\"\n",
      ex.file(),
      ex.line(),
      ex.function());
    fmt::print(stderr, "Message: \"{}\"\n", ex.what());
  }

  fmt::print("DONE.\n");
  return EXIT_SUCCESS;
}
