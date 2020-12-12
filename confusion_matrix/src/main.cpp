#include <cstdlib>

#include <fstream>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/algo/ranged_algorithms.hpp>
#include <pl/string_view.hpp>
#include <pl/unused.hpp>

#include <cl/fs/file.hpp>
#include <cl/use_unbuffered_io.hpp>

#include "confusion_matrix_best_configs.hpp"
#include "create_segmentation_results.hpp"
#include "csv_file_info.hpp"
#include "fetch.hpp"
#include "interpolated_data_set_paths.hpp"
#include "manual_segmentation_point.hpp"
#include "order_configurations_by_quality.hpp"

namespace {
/*!
 * \brief Constant for the output file to write to.
 **/
constexpr char outfile[] = "output.txt";

/*!
 * \brief Output file stream to write to `outfile`.
 **/
std::ofstream ofs{outfile, std::ios_base::out | std::ios_base::trunc};

/*!
 * \brief Prints the arguments given to stdout and `ofs`.
 * \tparam Args The types of the arguments given.
 * \param args The arguments
 * \return void
 **/
template<typename... Args>
decltype(auto) print(Args&&... args)
{
  fmt::print(args...);
  fmt::print(ofs, std::forward<Args>(args)...);
}
} // namespace

/*!
 * \brief Entry point.
 * \param argc Argument count.
 * \param argv Argument values.
 * \return EXIT_SUCCESS on success; otherwise EXIT_FAILURE.
 **/
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

    // This old code is based on the distance scores and doesn't work too well.
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

    std::vector<cm::ConfigWithTotalConfusionMatrix> bestConfigs{
      cm::confusionMatrixBestConfigs(
        manualSegmentationPoints,
        segmentationResults,
        cm::disregardTrueNegativesSorter)};
    print("disregardTrueNegativesSorter\n");
    for (const cm::ConfigWithTotalConfusionMatrix& cur : bestConfigs) {
      print("{}\n", cur);
    }
    print(
      "\nBest configuration (disregardTrueNegativesSorter): {}\n",
      bestConfigs.front());

    pl::algo::stable_sort(bestConfigs, cm::addTrueSubtractFalseSorter);
    print("addTrueSubtractFalse\n");
    for (const cm::ConfigWithTotalConfusionMatrix& cur : bestConfigs) {
      print("{}\n", cur);
    }
    print(
      "\nBest configuration (addTrueSubtractFalse): {}\n", bestConfigs.front());

#define SORT_PRINT(kind)                                              \
  pl::algo::stable_sort(bestConfigs, cm::kind##Sorter);               \
  print("{}\n", #kind);                                               \
  for (const cm::ConfigWithTotalConfusionMatrix& cur : bestConfigs) { \
    print("{}\n", cur);                                               \
  }                                                                   \
  print("\nBest configuration (" #kind "): {}\n", bestConfigs.front())

    SORT_PRINT(truePositives);
    SORT_PRINT(trueNegatives);
    SORT_PRINT(falsePositives);
    SORT_PRINT(falseNegatives);
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
