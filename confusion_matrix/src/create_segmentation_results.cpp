#include <future>
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

[[nodiscard]] unsigned hardwareThreads()
{
  unsigned result{std::thread::hardware_concurrency()};
  if (result == 0) { result = 4; }
  return result;
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

  const std::vector<std::string> segmentationKinds{"both"};

  const std::size_t totalCount{
    Configuration::skipWindowOptions().size()
    * Configuration::deleteTooCloseOptions().size()
    * Configuration::deleteTooLowVarianceOptions().size()
    * Configuration::imuOptions().size() * segmentationKinds.size()
    * Configuration::windowSizeOptions().size()
    * Configuration::filterKindOptions().size()};
  std::size_t i{0};

  fmt::print(
    "Importing segmentation points {} / {} ({:.2f}%) from Python...\r",
    i,
    totalCount,
    percentageOf(i, totalCount));

  std::vector<std::future<std::pair<
    Configuration,
    std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>>>
    futures{};

  for (bool skipWindowOption : Configuration::skipWindowOptions()) {
    for (bool deleteTooCloseOption : Configuration::deleteTooCloseOptions()) {
      for (bool deleteTooLowVarianceOption :
           Configuration::deleteTooLowVarianceOptions()) {
        for (Imu imuOption : Configuration::imuOptions()) {
          for (const std::string& segmentationKindOption : segmentationKinds) {
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
                  futures.push_back(
                    std::async(std::launch::async, [configuration] {
                      auto map{segment(configuration)};

                      if (!configuration.serializeSegmentationPoints(map)) {
                        CL_THROW_FMT(
                          "Could not serialize to \"{}\"!",
                          configuration.createFilePath());
                      }

                      return std::make_pair(configuration, std::move(map));
                    }));

                  if ((i % hardwareThreads()) == 0U) {
                    for (std::future<std::pair<
                           Configuration,
                           std::unordered_map<
                             cl::fs::Path,
                             std::vector<std::uint64_t>>>>& future : futures) {
                      segmentationResults.insert(future.get());
                    }

                    futures.clear();
                  }
                }

                ++i;
                fmt::print(
                  "Importing segmentation points {} / {} ({:.2f}%) from "
                  "Python...\r",
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

  for (std::future<std::pair<
         Configuration,
         std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>>&
         future : futures) {
    segmentationResults.insert(future.get());
  }

  futures.clear();

  fmt::print(
    "Importing segmentation points {} / {} ({:.2f}%) from Python...\n",
    totalCount,
    totalCount,
    percentageOf(totalCount, totalCount));

  return segmentationResults;
}
} // namespace cm
