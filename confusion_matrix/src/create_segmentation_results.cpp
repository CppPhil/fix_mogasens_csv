#include <thread>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/thd/thread_pool.hpp>

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

                futures.push_back(threadPool.add_task([configuration] {
                  return std::make_pair(configuration, segment(configuration));
                }));
              }
            }
          }
        }
      }
    }
  }

  const std::size_t futureCount{futures.size()};

  std::unordered_map<
    Configuration,
    std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>
    segmentationResults{};

  std::size_t i{0};

  for (std::future<std::pair<
         Configuration,
         std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>>&
         future : futures) {
    segmentationResults.insert(future.get());
    ++i;

    fmt::print(
      "{} / {} ({:.2f}%)\n", i, futureCount, percentageOf(i, futureCount));
  }

  fmt::print("{} / {} ({:.2f}%)\n", futureCount, futureCount, 100.0L);

  return segmentationResults;
}
} // namespace cm
