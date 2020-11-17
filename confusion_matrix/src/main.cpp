#include <cstdlib>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/string_view.hpp>
#include <pl/unused.hpp>

#include <cl/use_unbuffered_io.hpp>

#include "create_segmentation_results.hpp"
#include "fetch.hpp"
#include "find_best_configuration.hpp"
#include "manual_segmentation_point.hpp"

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

    std::uint64_t           bestConfigDistScore{};
    const cm::Configuration bestConfig{cm::findBestConfiguration(
      manualSegmentationPoints, segmentationResults, &bestConfigDistScore)};

    fmt::print(
      "\nbest config supposedly is:\n{}\ndistance score: {}\n",
      bestConfig,
      bestConfigDistScore);

    /*
    for (const auto& [path, segmentationPoints] :
         cm::fetch(segmentationResults, exampleConfig)) {
      const cm::DataSetIdentifier dsi{cm::toDataSetIdentifier(path)};

      constexpr pl::string_view prefix{"resources/preprocessed/Interpolated/"};
      pl::string_view           pathSv{path.str()};
      pathSv.remove_prefix(prefix.size());

      fmt::print(
        "Python: \"{:<45}\": [{}]\n",
        pathSv,
        fmt::join(segmentationPoints, ", "));
      fmt::print(
        "Manual: \"{:<45}\": [{}]\n",
        dsi,
        fmt::join(cm::fetch(manualSegmentationPoints, dsi), ", "));
      fmt::print("\n");
    }
*/
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
