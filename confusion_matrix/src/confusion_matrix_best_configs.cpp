#include <algorithm>
#include <atomic>
#include <ostream>
#include <thread>
#include <type_traits>
#include <utility>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/algo/ranged_algorithms.hpp>
#include <pl/meta/nested_types.hpp>
#include <pl/meta/remove_cvref.hpp>
#include <pl/numeric.hpp>
#include <pl/thd/thread_pool.hpp>

#include <cl/exception.hpp>

#include "confusion_matrix_best_configs.hpp"
#include "csv_file_info.hpp"
#include "fetch.hpp"

namespace cm {
namespace {
/*!
 * \brief Checks if `container` contains `value`.
 * \tparam Container type of `container`.
 * \tparam Value type of `value`.
 * \param container The container to find `value` in.
 * \param value The value to look for.
 * \return true if `container` contains `value`; otherwise false.
 **/
template<typename Container, typename Value>
[[nodiscard]] bool contains(const Container& container, const Value& value)
{
  const auto it{pl::algo::find(container, value)};
  return it != std::end(container);
}

/*!
 * \brief Checks if a uint64_t exists in `container` within a given delta.
 * \tparam Container The type of `container`.
 * \param container A container of uint64_t objects.
 * \param value The value to find in `container` within a delta.
 * \return true if `value` exists in `container` within a delte;
 *         false otherwise.
 **/
template<typename Container>
[[nodiscard]] bool existsWithinDelta(
  const Container& container,
  std::uint64_t    value)
{
  static_assert(
    std::is_same_v<
      pl::meta::value_type<pl::meta::remove_cvref_t<Container>>,
      std::uint64_t>,
    "Container in function template existsWithinDelta in file "
    "confusion_matrix_best_configs.cpp did not have std::uint64_t as its "
    "value_type.");

  // TODO: This delta might need to change. (450 milliseconds seems to be OK)
  static constexpr std::uint64_t deltaMs{450}; /* milliseconds */

  return pl::algo::any_of(container, [value](std::uint64_t element) {
    return pl::is_between(value, element - deltaMs, element + deltaMs);
  });
}
} // namespace

std::ostream& operator<<(
  std::ostream&                         os,
  const ConfigWithTotalConfusionMatrix& obj)
{
  const auto& mat{obj.matrix}; /* matrix */

  // Lambda to calculate the percentages.
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

ConfigWithTotalConfusionMatrix::ConfigWithTotalConfusionMatrix(
  Configuration   p_config,
  ConfusionMatrix p_matrix)
  : config{std::move(p_config)}, matrix{std::move(p_matrix)}
{
}

std::vector<ConfigWithTotalConfusionMatrix> confusionMatrixBestConfigs(
  const std::unordered_map<DataSetIdentifier, std::vector<std::uint64_t>>&
    manualSegmentationPoints,
  const std::unordered_map<
    Configuration,
    std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>&
                                             algorithmicallyDeterminedSegmentationPoints,
  const std::function<bool(
    const ConfigWithTotalConfusionMatrix&,
    const ConfigWithTotalConfusionMatrix&)>& sorter)
{
  // A std::function may be in a 'null' state.
  if (!sorter) { CL_THROW("sorter did not contain a valid target!"); }

  // std::thread::hardware_concurrency returns 0 on error, in that case we just
  // use 4 threads.
  pl::thd::thread_pool threadPool{
    std::max(std::thread::hardware_concurrency(), 4U)};
  std::vector<std::future<ConfigWithTotalConfusionMatrix>> futures{};

  std::atomic_size_t i{
    0}; // Atomic counter for the number of the current configuration.
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
        ConfusionMatrix configMatrix{}; // Buffer to accumulate into

        for (const auto& [csvFilePath, pythonSegmentationPoints] : map) {
          const DataSetIdentifier dsi{toDataSetIdentifier(csvFilePath)};
          const std::vector<std::uint64_t>& manualSegPoints{
            fetch(manualSegmentationPoints, dsi)};
          const CsvFileInfo csvFileInfo{csvFilePath};
          ConfusionMatrix   pathConfusionMatrix{};

          for (const std::uint64_t timestamp :
               csvFileInfo.hardwareTimestamps()) {
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
  pl::algo::transform( // join
    futures,
    result.begin(),
    [](auto& fut) { return fut.get(); });

  pl::algo::stable_sort(result, sorter);

  fmt::print(
    "                                                                   \r");
  fmt::print("Confusion matrices created.\n");

  return result;
}
} // namespace cm
