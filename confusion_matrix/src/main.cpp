#include <cstdlib>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/unused.hpp>

#include "create_segmentation_results.hpp"
#include "manual_segmentation_point.hpp"

int main(int argc, char* argv[])
{
  PL_UNUSED(argc);
  fmt::print("{}: Starting.\n", argv[0]);

  try { // TODO: HERE
    // TODO: This is just experimental.
    [[maybe_unused]] const std::unordered_map<
      cm::DataSetIdentifier,
      std::vector<cm::ManualSegmentationPoint>>
      manualSegmentationPointsMap{cm::ManualSegmentationPoint::readCsvFile()};

    [[maybe_unused]] const std::unordered_map<
      cm::Configuration,
      std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>
      segmentationResults{cm::createSegmentationResults()};

    for (const auto& [config, map] : segmentationResults) {
      fmt::print("{}\n", config);

      for (const auto& [csvFilePath, segmentationPoints] : map) {
        fmt::print(
          "{}: [{}]\n", csvFilePath, fmt::join(segmentationPoints, ", "));
      }

      fmt::print("\n\n");
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
