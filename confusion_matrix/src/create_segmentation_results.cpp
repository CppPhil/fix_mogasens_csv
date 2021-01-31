#include <future>
#include <thread>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/unreachable.hpp>

#include <cl/exception.hpp>

#include "create_segmentation_results.hpp"
#include "segment.hpp"

namespace cm {
namespace {
/*!
 * \brief Calculates what percentage `x` is of `y`.
 * \param x The first argument.
 * \param y The second argument.
 * \return The percentage that `x` is of `y`.
 *
 * Calculates what percentage `x` is of `y`.
 * E.g., if `x` is 2 and `y` is 6 the return value will be 33.333333333, because
 * `x` is one third of `y`.
 **/
[[nodiscard]] long double percentageOf(std::size_t x, std::size_t y)
{
  return (static_cast<long double>(x) / static_cast<long double>(y)) * 100.0L;
}

/*!
 * \brief Returns the amount of hardware threads.
 * \return The amount of hardware threads.
 **/
[[nodiscard]] unsigned hardwareThreads()
{
  unsigned result{std::thread::hardware_concurrency()};
  if (result == 0) { result = 4; } // Just use 4 threads on error.
  return result;
}

/*!
 * \brief Creates dots.
 * \param i How many dots to create.
 * \return Maps `i` to the corresponding amount of dots.
 **/
[[nodiscard]] std::string makeDots(std::size_t i)
{
  switch (i % 3U) {
  case 0U: return ".  ";
  case 1U: return ".. ";
  case 2U: return "...";
  }
  PL_UNREACHABLE();
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

  // We use a custom vector of strings for the segmentation kinds
  // rather than the one defined by `Configuration`, because
  // we only want to consider configurations using "both" as the
  // segmentation kind as the ground truth is based on that
  // same segmentation kind as well.
  const std::vector<std::string> segmentationKinds{"both"};

  // The total count of configurations.
  const std::size_t totalCount{
    Configuration::skipWindowOptions().size()
    * Configuration::deleteTooCloseOptions().size()
    * Configuration::deleteTooLowVarianceOptions().size()
    * Configuration::imuOptions().size() * segmentationKinds.size()
    * Configuration::windowSizeOptions().size()
    * Configuration::filterKindOptions().size()};
  std::size_t i{0}; // Counter for the current configuration.

  fmt::print(
    "Importing segmentation points from Python. Configuration {} "
    "/ {} ({:.2f}%)...\r",
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

                // If the file exists we just read the cached result from it.
                if (configuration.createFilePath().isFile()) {
                  std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>
                    fromFile{configuration.importSegmentationPoints()};
                  segmentationResults[configuration] = std::move(fromFile);
                }
                else {
                  // Otherwise we have to generate the result.
                  futures.push_back(
                    std::async(std::launch::async, [configuration] {
                      auto map{segment(
                        configuration)}; /* Invoke the Python segmentor */

                      // Try to write the result to the file.
                      if (!configuration.serializeSegmentationPoints(map)) {
                        CL_THROW_FMT(
                          "Could not serialize to \"{}\"!",
                          configuration.createFilePath());
                      }

                      return std::make_pair(configuration, std::move(map));
                    }));

                  // For each batch of hardwareThreads count, join the futures.
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
                  "Importing segmentation points from Python. Configuration {} "
                  "/ {} ({:.2f}%){}\r",
                  i,
                  totalCount,
                  percentageOf(i, totalCount),
                  makeDots(i));
              }
            }
          }
        }
      }
    }
  }

  // Join any remaining futures.
  for (std::future<std::pair<
         Configuration,
         std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>>&
         future : futures) {
    segmentationResults.insert(future.get());
  }

  futures.clear();

  fmt::print(
    "Importing segmentation points from Python. Configuration {} "
    "/ {} ({:.2f}%)...\n",
    totalCount,
    totalCount,
    percentageOf(totalCount, totalCount));

  return segmentationResults;
}
} // namespace cm
