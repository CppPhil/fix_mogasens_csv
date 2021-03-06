#include <cassert>
#include <cstddef>

#include <string>
#include <utility>

#include <tl/casts.hpp>

#include <pl/algo/ranged_algorithms.hpp>

#include "cl/sensor.hpp"
#include "cl/to_string.hpp"

#include "csv_line.hpp"

namespace cs {
namespace {
/*
 * \brief Attempts to convert a sensor ID encoded as an ExtractId to a string.
 * \param sensorId The sensor ID encoded as an ExtractId to convert.
 * \param ok Output parameter for the status code of this function.
 * \return The resulting string
 * \warning The return value will be an empty string if *ok == false;
 *          it will only contain a valid value if *ok == true.
 * \note Will only succeed if `sensorId` is 769, 770, 771 or 772.
 **/
[[nodiscard]] std::string sensorIdToString(std::uint64_t sensorId, bool* ok)
{
  *ok = false;

  const auto it{pl::algo::find_if(cl::sensors, [sensorId](cl::Sensor sensor) {
    return tl::underlying_cast(sensor) == sensorId;
  })};

  if (it == cl::sensors.end()) { return ""; }

  std::string result{cl::to_string(*it)};
  *ok = true;

  return result;
}
} // namespace

CsvLineBuilder::CsvLineBuilder()
  : m_skipWindow{tl::nullopt}
  , m_deleteTooClose{tl::nullopt}
  , m_deleteLowVariance{tl::nullopt}
  , m_kind{tl::nullopt}
  , m_windowSize{tl::nullopt}
  , m_filter{tl::nullopt}
  , m_dataSet{tl::nullopt}
  , m_sensor{tl::nullopt}
  , m_repetitions{tl::nullopt}
  , m_segmentationPoints{tl::nullopt}
{
}

CsvLineBuilder& CsvLineBuilder::skipWindow(bool value)
{
  m_skipWindow = value;
  return *this;
}

CsvLineBuilder& CsvLineBuilder::deleteTooClose(bool value)
{
  m_deleteTooClose = value;
  return *this;
}

CsvLineBuilder& CsvLineBuilder::deleteLowVariance(bool value)
{
  m_deleteLowVariance = value;
  return *this;
}

CsvLineBuilder& CsvLineBuilder::kind(SegmentationKind value)
{
  m_kind = value;
  return *this;
}

CsvLineBuilder& CsvLineBuilder::windowSize(std::uint64_t value)
{
  m_windowSize = value;
  return *this;
}

CsvLineBuilder& CsvLineBuilder::filter(FilterKind value)
{
  m_filter = value;
  return *this;
}

CsvLineBuilder& CsvLineBuilder::dataSet(std::string value)
{
  m_dataSet = std::move(value);
  return *this;
}

CsvLineBuilder& CsvLineBuilder::sensor(std::uint64_t value)
{
  m_sensor = value;
  return *this;
}

CsvLineBuilder& CsvLineBuilder::repetitions(std::uint64_t value)
{
  m_repetitions = value;
  return *this;
}

CsvLineBuilder& CsvLineBuilder::segmentationPoints(std::uint64_t value)
{
  m_segmentationPoints = value;
  return *this;
}

CsvLineBuilder& CsvLineBuilder::isOld(bool value)
{
  m_isOld = value;
  return *this;
}

std::vector<std::string> CsvLineBuilder::build() const
{
  assert(m_skipWindow.has_value());
  assert(m_deleteTooClose.has_value());
  assert(m_deleteLowVariance.has_value());
  assert(m_kind.has_value());
  assert(m_windowSize.has_value());
  assert(m_filter.has_value());
  assert(m_dataSet.has_value());
  assert(m_sensor.has_value());
  assert(m_repetitions.has_value());
  assert(m_segmentationPoints.has_value());
  assert(m_isOld.has_value());

  constexpr std::size_t lineLength{11};

  constexpr std::size_t skipWindowIndex{0};
  constexpr std::size_t deleteTooCloseIndex{1};
  constexpr std::size_t deleteLowVarianceIndex{2};
  constexpr std::size_t kindIndex{3};
  constexpr std::size_t windowSizeIndex{4};
  constexpr std::size_t filterIndex{5};
  constexpr std::size_t dataSetIndex{6};
  constexpr std::size_t sensorIndex{7};
  constexpr std::size_t repetitionsIndex{8};
  constexpr std::size_t segmentationPointsIndex{9};
  constexpr std::size_t isOldIndex{10};

  std::vector<std::string> line(lineLength, "");

  [[maybe_unused]] bool ok{false};
  std::string           sensorString{sensorIdToString(m_sensor.value(), &ok)};

  assert(ok && "Invalid sensor ID!");

  line[skipWindowIndex]         = cl::to_string(m_skipWindow.value());
  line[deleteTooCloseIndex]     = cl::to_string(m_deleteTooClose.value());
  line[deleteLowVarianceIndex]  = cl::to_string(m_deleteLowVariance.value());
  line[kindIndex]               = cl::to_string(m_kind.value());
  line[windowSizeIndex]         = cl::to_string(m_windowSize.value());
  line[filterIndex]             = cl::to_string(m_filter.value());
  line[dataSetIndex]            = m_dataSet.value();
  line[sensorIndex]             = std::move(sensorString);
  line[repetitionsIndex]        = cl::to_string(m_repetitions.value());
  line[segmentationPointsIndex] = cl::to_string(m_segmentationPoints.value());
  line[isOldIndex]              = m_isOld.value() ? "old" : "preprocessed";

  return line;
}
} // namespace cs
