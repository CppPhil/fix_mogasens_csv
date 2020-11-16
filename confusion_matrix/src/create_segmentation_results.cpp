#include <thread>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <cl/exception.hpp>

#include "create_segmentation_results.hpp"
#include "segment.hpp"

namespace cm {
namespace {
[[nodiscard]] long double percentageOf(std::size_t x, std::size_t y)
{
  return (static_cast<long double>(x) / static_cast<long double>(y)) * 100.0L;
}
} // namespace

std::unordered_map<
  Configuration,
  std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>
createSegmentationResults()
{
  std::unordered_map<
    Configuration,
    std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>
    segmentationResults{};

  const std::size_t totalCount{
    Configuration::skipWindowOptions().size()
    * Configuration::deleteTooCloseOptions().size()
    * Configuration::deleteTooLowVarianceOptions().size()
    * Configuration::imuOptions().size()
    * Configuration::segmentationKindOptions().size()
    * Configuration::windowSizeOptions().size()
    * Configuration::filterKindOptions().size()};
  std::size_t i{0};

  fmt::print("{} / {} ({:.2f}%)\r", i, totalCount, percentageOf(i, totalCount));

  for (bool skipWindowOption : Configuration::skipWindowOptions()) {
    for (bool deleteTooCloseOption : Configuration::deleteTooCloseOptions()) {
      for (bool deleteTooLowVarianceOption :
           Configuration::deleteTooLowVarianceOptions()) {
        for (Imu imuOption : Configuration::imuOptions()) {
          for (const std::string& segmentationKindOption :
               Configuration::segmentationKindOptions()) {
            for (std::size_t windowSizeOption :
                 Configuration::windowSizeOptions()) {
              for (const std::string& filterKindOption :
                   Configuration::filterKindOptions()) {
                const Configuration configuration{
                  Configuration::Builder{}
                    .skipWindow(skipWindowOption)
                    .deleteTooClose(deleteTooCloseOption)
                    .deleteTooLowVariance(deleteTooLowVarianceOption)
                    .imu(imuOption)
                    .segmentationKind(segmentationKindOption)
                    .windowSize(windowSizeOption)
                    .filterKind(filterKindOption)
                    .build()};

                if (configuration.createFilePath().isFile()) {
                  std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>
                    fromFile{configuration.importSegmentationPoints()};
                  segmentationResults[configuration] = std::move(fromFile);
                }
                else {
                  std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>
                    map{segment(configuration)};

                  if (!configuration.serializeSegmentationPoints(map)) {
                    CL_THROW_FMT(
                      "Could not serialize to \"{}\"!",
                      configuration.createFilePath());
                  }

                  segmentationResults[configuration] = std::move(map);
                }

                ++i;
                fmt::print(
                  "{} / {} ({:.2f}%)\r",
                  i,
                  totalCount,
                  percentageOf(i, totalCount));
              }
            }
          }
        }
      }
    }
  }

  fmt::print(
    "{} / {} ({:.2f}%)\n",
    totalCount,
    totalCount,
    percentageOf(totalCount, totalCount));

  return segmentationResults;
}
} // namespace cm
