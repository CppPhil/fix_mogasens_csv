#include <algorithm>
#include <atomic>
#include <ostream>
#include <thread>
#include <utility>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/algo/ranged_algorithms.hpp>
#include <pl/numeric.hpp>
#include <pl/thd/thread_pool.hpp>

#include "confusion_matrix_best_configs.hpp"
#include "csv_file_info.hpp"
#include "fetch.hpp"

namespace cm {
namespace {
template<typename Container, typename Value>
[[nodiscard]] bool contains(const Container& container, const Value& value)
{
  const auto it{pl::algo::find(container, value)};
  return it != std::end(container);
}

template<typename Container, typename Value>
[[nodiscard]] bool existsWithinDelta(
  const Container& container,
  const Value&     value)
{
  // TODO: This might need to change.
  constexpr std::uint64_t deltaMs{450};

  return pl::algo::any_of(container, [value](std::uint64_t element) {
    return pl::is_between(value, element - deltaMs, element + deltaMs);
  });
}
} // namespace

ConfigWithTotalConfusionMatrix::ConfigWithTotalConfusionMatrix(
  Configuration   p_config,
  ConfusionMatrix p_matrix)
  : config{std::move(p_config)}, matrix{std::move(p_matrix)}
{
}

bool operator<(
  const ConfigWithTotalConfusionMatrix& lhs,
  const ConfigWithTotalConfusionMatrix& rhs) noexcept
{
  const std::uint64_t lhsValue{
    lhs.matrix.truePositives() + lhs.matrix.trueNegatives()
    - lhs.matrix.falsePositives() - lhs.matrix.falseNegatives()};
  const std::uint64_t rhsValue{
    rhs.matrix.truePositives() + rhs.matrix.trueNegatives()
    - rhs.matrix.falsePositives() - rhs.matrix.falseNegatives()};

  return lhsValue < rhsValue;
}

std::ostream& operator<<(
  std::ostream&                         os,
  const ConfigWithTotalConfusionMatrix& obj)
{
  const auto& mat{obj.matrix};

  const auto percent = [&mat](std::uint64_t val) {
    return static_cast<long double>(val) / mat.totalCount() * 100.0L;
  };

  return os << fmt::format(
           "tp: {} ({:.2f}%), tn: {} ({:.2f}%), fp: {} ({:.2f}%), fn: {} "
           "({:.2f}%),\nconfig: {}",
           mat.truePositives(),
           percent(mat.truePositives()),
           mat.trueNegatives(),
           percent(mat.trueNegatives()),
           mat.falsePositives(),
           percent(mat.falsePositives()),
           mat.falseNegatives(),
           percent(mat.falseNegatives()),
           obj.config);
}

std::vector<ConfigWithTotalConfusionMatrix> confusionMatrixBestConfigs(
  const std::unordered_map<DataSetIdentifier, std::vector<std::uint64_t>>&
    manualSegmentationPoints,
  const std::unordered_map<
    Configuration,
    std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>&
    algorithmicallyDeterminedSegmentationPoints)
{
  pl::thd::thread_pool threadPool{
    std::max(std::thread::hardware_concurrency(), 4U)};
  std::vector<std::future<ConfigWithTotalConfusionMatrix>> futures{};

  std::atomic_size_t i{0};
  for (const std::pair<
         const Configuration,
         std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>& pair :
       algorithmicallyDeterminedSegmentationPoints) {
    const Configuration& config{pair.first};
    const std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>& map{
      pair.second};
    futures.push_back(
      threadPool.add_task([&map,
                           &manualSegmentationPoints,
                           &config,
                           &algorithmicallyDeterminedSegmentationPoints,
                           &i] {
        ConfusionMatrix configMatrix{};

        for (const auto& [csvFilePath, pythonSegmentationPoints] : map) {
          const DataSetIdentifier dsi{toDataSetIdentifier(csvFilePath)};
          const std::vector<std::uint64_t>& manualSegPoints{
            fetch(manualSegmentationPoints, dsi)};
          const CsvFileInfo csvFileInfo{csvFilePath};
          ConfusionMatrix   pathConfusionMatrix{};

          for (std::uint64_t timestamp{csvFileInfo.hardwareTimestampBegin()};
               timestamp <= csvFileInfo.hardwareTimestampEnd();
               timestamp += csvFileInfo.hardwareTimestampStepSize()) {
            if (contains(pythonSegmentationPoints, timestamp)) {
              if (existsWithinDelta(manualSegPoints, timestamp)) {
                pathConfusionMatrix.incrementTruePositives();
              }
              else {
                pathConfusionMatrix.incrementFalsePositives();
              }
            }
            else if (
              contains(manualSegPoints, timestamp)
              && !existsWithinDelta(pythonSegmentationPoints, timestamp)) {
              pathConfusionMatrix.incrementFalseNegatives();
            }
            else {
              pathConfusionMatrix.incrementTrueNegatives();
            }
          }

          configMatrix += pathConfusionMatrix;
        }

        ++i;
        fmt::print(
          "confusion matrix for config {} / {} ({:.2f}%) created.\r",
          i,
          algorithmicallyDeterminedSegmentationPoints.size(),
          static_cast<long double>(i)
            / algorithmicallyDeterminedSegmentationPoints.size() * 100.0L);

        return ConfigWithTotalConfusionMatrix{config, configMatrix};
      }));
  }

  std::vector<ConfigWithTotalConfusionMatrix> result(futures.size());
  pl::algo::transform(
    futures, result.begin(), [](auto& fut) { return fut.get(); });

  pl::algo::sort(result);

  fmt::print(
    "                                                                   \r");
  fmt::print("Confusion matrices created.\n");

  return result;
}
} // namespace cm
