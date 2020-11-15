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

    std::unordered_map<
      cm::Configuration,
      std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>
      segmentationResults{};

    for (bool skipWindowOption : cm::Configuration::skipWindowOptions()) {
      for (bool deleteTooCloseOption :
           cm::Configuration::deleteTooCloseOptions()) {
        for (bool deleteTooLowVarianceOption :
             cm::Configuration::deleteTooLowVarianceOptions()) {
          for (cm::Imu imuOption : cm::Configuration::imuOptions()) {
            for (const std::string& segmentationKindOption :
                 cm::Configuration::segmentationKindOptions()) {
              for (std::size_t windowSizeOption :
                   cm::Configuration::windowSizeOptions()) {
                for (const std::string& filterKindOption :
                     cm::Configuration::filterKindOptions()) {
                  const cm::Configuration configuration{
                    cm::Configuration::Builder{}
                      .skipWindow(skipWindowOption)
                      .deleteTooClose(deleteTooCloseOption)
                      .deleteTooLowVariance(deleteTooLowVarianceOption)
                      .imu(imuOption)
                      .segmentationKind(segmentationKindOption)
                      .windowSize(windowSizeOption)
                      .filterKind(filterKindOption)
                      .build()};
                  segmentationResults[configuration]
                    = cm::segment(configuration);
                }
              }
            }
          }
        }
      }
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
