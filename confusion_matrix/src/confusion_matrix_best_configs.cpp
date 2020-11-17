#include <ostream>
#include <utility>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/algo/ranged_algorithms.hpp>
#include <pl/numeric.hpp>

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
  constexpr std::uint64_t deltaMs{150};

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
  return os << fmt::format(
           "tp: {}, tn: {}, fp: {}, fn: {},\nconfig: {}",
           obj.matrix.truePositives(),
           obj.matrix.trueNegatives(),
           obj.matrix.falsePositives(),
           obj.matrix.falseNegatives(),
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
  std::vector<ConfigWithTotalConfusionMatrix> result{};

  std::size_t i{0};
  for (const auto& [config, map] :
       algorithmicallyDeterminedSegmentationPoints) {
    ConfusionMatrix configMatrix{};

    for (const auto& [csvFilePath, pythonSegmentationPoints] : map) {
      const DataSetIdentifier           dsi{toDataSetIdentifier(csvFilePath)};
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
    result.emplace_back(config, configMatrix);
  }

  pl::algo::sort(result);

  fmt::print(
    "                                                                   \r");
  fmt::print("Confusion matrices created.\n");

  return result;
}
} // namespace cm
