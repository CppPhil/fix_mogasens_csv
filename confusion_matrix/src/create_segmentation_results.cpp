#include <thread>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/thd/thread_pool.hpp>

#include <cl/exception.hpp>

#include "create_segmentation_results.hpp"
#include "segment.hpp"

namespace cm {
namespace {
[[nodiscard]] unsigned hardwareThreads()
{
  const unsigned value{std::thread::hardware_concurrency()};
  if (value == 0U) { return 4U; }
  return value;
}

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
  pl::thd::thread_pool threadPool{hardwareThreads()};

  fmt::print(
    "Created thread pool with {} threads.\n", threadPool.thread_count());

  std::unordered_map<
    Configuration,
    std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>
    segmentationResults{};

  std::vector<std::future<std::pair<
    Configuration,
    std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>>>
    futures{};

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
                  fmt::print(
                    "\"{}\" exists, importing segmentation points.\n",
                    configuration.createFilePath());
                  std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>
                    fromFile{configuration.importSegmentationPoints()};
                  segmentationResults[configuration] = std::move(fromFile);
                }
                else {
                  fmt::print(
                    "\"{}\" doesn't exist, adding task to thread pool.\n",
                    configuration.createFilePath());
                  futures.push_back(threadPool.add_task([configuration] {
                    return std::make_pair(
                      configuration, segment(configuration));
                  }));
                }
              }
            }
          }
        }
      }
    }
  }

  const std::size_t futureCount{futures.size()};

  fmt::print("Thread pool has {} tasks.\n", futureCount);

  std::size_t i{0};
  fmt::print(
    "{} / {} ({:.2f}%)\r", i, futureCount, percentageOf(i, futureCount));

  for (std::future<std::pair<
         Configuration,
         std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>>&
         future : futures) {
    std::pair<
      Configuration,
      std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>
                         futureResult{future.get()};
    const Configuration& configuration{futureResult.first};
    const std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>& map{
      futureResult.second};

    if (!configuration.serializeSegmentationPoints(map)) {
      CL_THROW_FMT(
        "Could not serialize to \"{}\"!", configuration.createFilePath());
    }

    segmentationResults.insert(std::move(futureResult));
    ++i;

    fmt::print(
      "{} / {} ({:.2f}%)\r", i, futureCount, percentageOf(i, futureCount));
  }

  fmt::print("{} / {} ({:.2f}%)\n", futureCount, futureCount, 100.0L);

  return segmentationResults;
}
} // namespace cm
