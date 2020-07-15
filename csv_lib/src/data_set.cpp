#include <cassert>
#include <cmath>
#include <cstdint>

#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#include <fmt/format.h>

#include <pl/algo/ranged_algorithms.hpp>
#include <pl/assert.hpp>

#include "data_set.hpp"
#include "sensor.hpp"

namespace cl {
namespace {
template<typename To>
inline To convert(const std::string& inputString)
{
  if constexpr (std::is_same_v<To, long double>) {
    return std::stold(inputString); // may throw
  }
  else if constexpr (
    std::is_same_v<To, std::uint64_t> || std::is_same_v<To, Sensor>) {
    constexpr int decimalBase{10};
    return To{std::stoull( // may throw
      /* str */ inputString,
      /* pos */ nullptr,
      /* base*/ decimalBase)};
  }
  else {
    assert(false && "data_set.cpp convert: unreachable branch reached!");
    throw std::invalid_argument{
      "data_set.cpp convert: unreachable branch reached!"};
  }
}
} // namespace

[[nodiscard]] Expected<DataSet> DataSet::create(
  std::string                                  fileName,
  const std::vector<std::vector<std::string>>& matrix)
{
  if (matrix.empty()) {
    return CL_UNEXPECTED(Error::InvalidArgument, "The matrix is empty.");
  }

  static constexpr std::size_t expectedRowLength{10};
  if (pl::algo::any_of(matrix, [](const std::vector<std::string>& row) {
        return row.size() != expectedRowLength;
      })) {
    return CL_UNEXPECTED(
      Error::InvalidArgument,
      fmt::format(
        "One or more rows in the matrix don't have a length of {}.",
        expectedRowLength));
  }

  std::vector<column_type<Column::Time>>              time{};
  std::vector<column_type<Column::HardwareTimestamp>> hardwareTimestamp{};
  std::vector<column_type<Column::ExtractId>>         extractId{};
  std::vector<column_type<Column::Trigger>>           trigger{};
  std::vector<column_type<Column::AccelerometerX>>    accelerometerX{};
  std::vector<column_type<Column::AccelerometerY>>    accelerometerY{};
  std::vector<column_type<Column::AccelerometerZ>>    accelerometerZ{};
  std::vector<column_type<Column::GyroscopeX>>        gyroscopeX{};
  std::vector<column_type<Column::GyroscopeY>>        gyroscopeY{};
  std::vector<column_type<Column::GyroscopeZ>>        gyroscopeZ{};

  for (std::size_t rowIndex{0}; rowIndex < matrix.size(); ++rowIndex) {
    const std::vector<std::string>& currentRow{matrix[rowIndex]};

    for (std::size_t columnIndex{0}; columnIndex < currentRow.size();
         ++columnIndex) {
      const std::string& currentCell{currentRow[columnIndex]};

      try {
        switch (columnIndex) {
        case column_index<Column::Time>:
          time.push_back(convert<column_type<Column::Time>>(currentCell));
          break;
        case column_index<Column::HardwareTimestamp>:
          hardwareTimestamp.push_back(
            convert<column_type<Column::HardwareTimestamp>>(currentCell));
          break;
        case column_index<Column::ExtractId>:
          extractId.push_back(
            convert<column_type<Column::ExtractId>>(currentCell));
          break;
        case column_index<Column::Trigger>:
          trigger.push_back(convert<column_type<Column::Trigger>>(currentCell));
          break;
        case column_index<Column::AccelerometerX>:
          accelerometerX.push_back(
            convert<column_type<Column::AccelerometerX>>(currentCell));
          break;
        case column_index<Column::AccelerometerY>:
          accelerometerY.push_back(
            convert<column_type<Column::AccelerometerY>>(currentCell));
          break;
        case column_index<Column::AccelerometerZ>:
          accelerometerZ.push_back(
            convert<column_type<Column::AccelerometerZ>>(currentCell));
          break;
        case column_index<Column::GyroscopeX>:
          gyroscopeX.push_back(
            convert<column_type<Column::GyroscopeX>>(currentCell));
          break;
        case column_index<Column::GyroscopeY>:
          gyroscopeY.push_back(
            convert<column_type<Column::GyroscopeY>>(currentCell));
          break;
        case column_index<Column::GyroscopeZ>:
          gyroscopeZ.push_back(
            convert<column_type<Column::GyroscopeZ>>(currentCell));
          break;
        }
      }
      catch (const std::invalid_argument& ex) {
        return CL_UNEXPECTED(
          Error::InvalidArgument,
          fmt::format(
            "Conversion failure (std::invalid_argument caught): \"{}\"",
            ex.what()));
      }
      catch (const std::out_of_range& ex) {
        return CL_UNEXPECTED(
          Error::InvalidArgument,
          fmt::format(
            "Conversion failure (std::out_of_range caught): \"{}\"",
            ex.what()));
      }
    }
  }

  return DataSet{
    std::move(fileName),
    std::move(time),
    std::move(hardwareTimestamp),
    std::move(extractId),
    std::move(trigger),
    std::move(accelerometerX),
    std::move(accelerometerY),
    std::move(accelerometerZ),
    std::move(gyroscopeX),
    std::move(gyroscopeY),
    std::move(gyroscopeZ)};
}

DataSet::size_type DataSet::rowCount() const noexcept
{
  assert(
    (m_time.size() == m_hardwareTimestamp.size())
    && (m_time.size() == m_extractId.size())
    && (m_time.size() == m_trigger.size())
    && (m_time.size() == m_accelerometerX.size())
    && (m_time.size() == m_accelerometerY.size())
    && (m_time.size() == m_accelerometerZ.size())
    && (m_time.size() == m_gyroscopeX.size())
    && (m_time.size() == m_gyroscopeY.size())
    && (m_time.size() == m_gyroscopeZ.size())
    && "Vectors were not of equal size in DataSet::rowCount!");

  return m_time.size();
}

const std::string& DataSet::fileName() const noexcept { return m_fileName; }

column_type<Column::Time> DataSet::time(size_type index) const
{
  PL_DBG_CHECK_PRE(index < m_time.size());

  return m_time[index];
}

column_type<Column::HardwareTimestamp> DataSet::hardwareTimestamp(
  size_type index) const
{
  PL_DBG_CHECK_PRE(index < m_hardwareTimestamp.size());

  return m_hardwareTimestamp[index];
}

column_type<Column::ExtractId> DataSet::extractId(size_type index) const
{
  PL_DBG_CHECK_PRE(index < m_extractId.size());

  return m_extractId[index];
}

column_type<Column::Trigger> DataSet::trigger(size_type index) const
{
  PL_DBG_CHECK_PRE(index < m_trigger.size());

  return m_trigger[index];
}

column_type<Column::AccelerometerX> DataSet::accelerometerX(
  size_type index) const
{
  PL_DBG_CHECK_PRE(index < m_accelerometerX.size());

  return m_accelerometerX[index];
}

column_type<Column::AccelerometerY> DataSet::accelerometerY(
  size_type index) const
{
  PL_DBG_CHECK_PRE(index < m_accelerometerY.size());

  return m_accelerometerY[index];
}

column_type<Column::AccelerometerZ> DataSet::accelerometerZ(
  size_type index) const
{
  PL_DBG_CHECK_PRE(index < m_accelerometerZ.size());

  return m_accelerometerZ[index];
}

column_type<Column::GyroscopeX> DataSet::gyroscopeX(size_type index) const
{
  PL_DBG_CHECK_PRE(index < m_gyroscopeX.size());

  return m_gyroscopeX[index];
}

column_type<Column::GyroscopeY> DataSet::gyroscopeY(size_type index) const
{
  PL_DBG_CHECK_PRE(index < m_gyroscopeY.size());

  return m_gyroscopeY[index];
}

column_type<Column::GyroscopeZ> DataSet::gyroscopeZ(size_type index) const
{
  PL_DBG_CHECK_PRE(index < m_gyroscopeZ.size());

  return m_gyroscopeZ[index];
}

namespace {
constexpr std::array<DataSet::ChannelAccessor, 3> accelerometerAccessors{
  {&DataSet::accelerometerX,
   &DataSet::accelerometerY,
   &DataSet::accelerometerZ}};

constexpr std::array<DataSet::ChannelAccessor, 3> gyroscopeAccessors{
  {&DataSet::gyroscopeX, &DataSet::gyroscopeY, &DataSet::gyroscopeZ}};
} // namespace

long double DataSet::accelerometerAverage() const
{
  return average(accelerometerAccessors);
}

long double DataSet::gyroscopeAverage() const
{
  return average(gyroscopeAccessors);
}

long double DataSet::accelerometerMaximum() const
{
  return maximum(accelerometerAccessors);
}

long double DataSet::gyroscopeMaximum() const
{
  return maximum(gyroscopeAccessors);
}

DataSet::DataSet(
  std::string&&                                         fileName,
  std::vector<column_type<Column::Time>>&&              time,
  std::vector<column_type<Column::HardwareTimestamp>>&& hardwareTimestamp,
  std::vector<column_type<Column::ExtractId>>&&         extractId,
  std::vector<column_type<Column::Trigger>>&&           trigger,
  std::vector<column_type<Column::AccelerometerX>>&&    accelerometerX,
  std::vector<column_type<Column::AccelerometerY>>&&    accelerometerY,
  std::vector<column_type<Column::AccelerometerZ>>&&    accelerometerZ,
  std::vector<column_type<Column::GyroscopeX>>&&        gyroscopeX,
  std::vector<column_type<Column::GyroscopeY>>&&        gyroscopeY,
  std::vector<column_type<Column::GyroscopeZ>>&&        gyroscopeZ) noexcept
  : m_fileName{std::move(fileName)}
  , m_time{std::move(time)}
  , m_hardwareTimestamp{std::move(hardwareTimestamp)}
  , m_extractId{std::move(extractId)}
  , m_trigger{std::move(trigger)}
  , m_accelerometerX{std::move(accelerometerX)}
  , m_accelerometerY{std::move(accelerometerY)}
  , m_accelerometerZ{std::move(accelerometerZ)}
  , m_gyroscopeX{std::move(gyroscopeX)}
  , m_gyroscopeY{std::move(gyroscopeY)}
  , m_gyroscopeZ{std::move(gyroscopeZ)}
{
}

long double DataSet::average(
  const std::array<ChannelAccessor, 3>& accessors) const
{
  const size_type dataPointCount{accessors.size() * rowCount()};
  long double     accumulator{0.0L};

  for (size_type i{0}; i < rowCount(); ++i) {
    for (ChannelAccessor channelAccessor : accessors) {
      accumulator += std::abs((this->*channelAccessor)(i));
    }
  }

  return accumulator / static_cast<long double>(dataPointCount);
}

long double DataSet::maximum(
  const std::array<ChannelAccessor, 3>& accessors) const
{
  long double result{0.0L};

  for (size_type i{0}; i < rowCount(); ++i) {
    for (ChannelAccessor channelAccessor : accessors) {
      result = std::max(result, std::abs((this->*channelAccessor)(i)));
    }
  }

  return result;
}
} // namespace cl
