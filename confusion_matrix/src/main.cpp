#include <cstdlib>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/string_view.hpp>
#include <pl/unused.hpp>

#include <cl/use_unbuffered_io.hpp>

#include "confusion_matrix_best_configs.hpp"
#include "create_segmentation_results.hpp"
#include "csv_file_info.hpp"
#include "fetch.hpp"
#include "interpolated_data_set_paths.hpp"
#include "manual_segmentation_point.hpp"
#include "order_configurations_by_quality.hpp"

int main(int argc, char* argv[])
{
  PL_UNUSED(argc);
  cl::useUnbufferedIo();
  fmt::print("{}: Starting.\n", argv[0]);

  try { // TODO: HERE
    // TODO: This is just experimental.

    // Manual ones as offsets from video start
    const std::unordered_map<
      cm::DataSetIdentifier,
      std::vector<cm::ManualSegmentationPoint>>
      manualSegmentationPointsMap{cm::ManualSegmentationPoint::readCsvFile()};

    // Algorithmically determined ones from Python
    const std::unordered_map<
      cm::Configuration,
      std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>
      segmentationResults{cm::createSegmentationResults()};

    const cm::Configuration exampleConfig{cm::Configuration::Builder{}
                                            .skipWindow(false)
                                            .deleteTooClose(false)
                                            .deleteTooLowVariance(false)
                                            .imu(cm::Imu::Accelerometer)
                                            .segmentationKind("both")
                                            .windowSize(501U)
                                            .filterKind("butterworth")
                                            .build()};

    // Manual ones as hardware timestamps
    const std::unordered_map<cm::DataSetIdentifier, std::vector<std::uint64_t>>
      manualSegmentationPoints{
        cm::ManualSegmentationPoint::convertToHardwareTimestamps(
          /* manualSegmentationPoints */ manualSegmentationPointsMap,
          /* pythonResult */ cm::fetch(segmentationResults, exampleConfig))};

    /*
    const std::vector<cm::ConfigWithDistanceScore> bestConfigurations{
      cm::orderConfigurationsByQuality(
        manualSegmentationPoints, segmentationResults)};

    constexpr std::size_t configurationsToPrint{20};
    for (std::size_t i{0};
         (i < configurationsToPrint) && (i < bestConfigurations.size());
         ++i) {
      fmt::print("{}: {}\n", i, bestConfigurations[i]);
    }

    fmt::print("\nBest configuration: {}\n", bestConfigurations.front());
    */

    const std::vector<cm::ConfigWithTotalConfusionMatrix> bestConfigs{
      cm::confusionMatrixBestConfigs(
        manualSegmentationPoints, segmentationResults)};
    constexpr std::size_t configurationsToPrint{20};
    for (std::size_t i{0};
         (i < configurationsToPrint) && (i < bestConfigs.size());
         ++i) {
      fmt::print("{}: {}\n", i, bestConfigs[i]);
    }
    fmt::print("\nBest configuration: {}\n", bestConfigs.front());
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
    return EXIT_FAILURE;
  }
  catch (const std::exception& ex) {
    fmt::print(stderr, "{}: caught std::exception\n", PL_CURRENT_FUNCTION);
    fmt::print(stderr, "Message: \"{}\"\n", ex.what());
    return EXIT_FAILURE;
  }

  fmt::print("DONE.\n");
  return EXIT_SUCCESS;
}
