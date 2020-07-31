#ifndef INCG_CL_DATA_SET_HPP
#define INCG_CL_DATA_SET_HPP
#include <cstddef>

#include <array>
#include <string>
#include <vector>

#include "cl/column.hpp"
#include "cl/error.hpp"
#include "cl/sensor.hpp"

namespace cl {
class DataSet {
public:
  using size_type       = std::size_t;
  using ChannelAccessor = long double (DataSet::*)(size_type) const;

  [[nodiscard]] static Expected<DataSet> create(
    std::string                                  fileName,
    const std::vector<std::vector<std::string>>& matrix);

  size_type rowCount() const noexcept;

  const std::string& fileName() const noexcept;

  column_type<Column::Time> time(size_type index) const;

  column_type<Column::HardwareTimestamp> hardwareTimestamp(
    size_type index) const;

  column_type<Column::ExtractId> extractId(size_type index) const;

  column_type<Column::Trigger> trigger(size_type index) const;

  column_type<Column::AccelerometerX> accelerometerX(size_type index) const;

  column_type<Column::AccelerometerY> accelerometerY(size_type index) const;

  column_type<Column::AccelerometerZ> accelerometerZ(size_type index) const;

  column_type<Column::GyroscopeX> gyroscopeX(size_type index) const;

  column_type<Column::GyroscopeY> gyroscopeY(size_type index) const;

  column_type<Column::GyroscopeZ> gyroscopeZ(size_type index) const;

  long double accelerometerAverage(Sensor sensor) const;

  long double gyroscopeAverage(Sensor sensor) const;

  long double accelerometerMaximum(Sensor sensor) const;

  long double gyroscopeMaximum(Sensor sensor) const;

private:
  explicit DataSet(
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
    std::vector<column_type<Column::GyroscopeZ>>&&        gyroscopeZ) noexcept;

  long double average(
    const std::array<ChannelAccessor, 3>& accessors,
    Sensor                                sensor) const;

  long double maximum(
    const std::array<ChannelAccessor, 3>& accessors,
    Sensor                                sensor) const;

  std::string                                         m_fileName;
  std::vector<column_type<Column::Time>>              m_time;
  std::vector<column_type<Column::HardwareTimestamp>> m_hardwareTimestamp;
  std::vector<column_type<Column::ExtractId>>         m_extractId;
  std::vector<column_type<Column::Trigger>>           m_trigger;
  std::vector<column_type<Column::AccelerometerX>>    m_accelerometerX;
  std::vector<column_type<Column::AccelerometerY>>    m_accelerometerY;
  std::vector<column_type<Column::AccelerometerZ>>    m_accelerometerZ;
  std::vector<column_type<Column::GyroscopeX>>        m_gyroscopeX;
  std::vector<column_type<Column::GyroscopeY>>        m_gyroscopeY;
  std::vector<column_type<Column::GyroscopeZ>>        m_gyroscopeZ;
};
} // namespace cl
#endif // INCG_CL_DATA_SET_HPP
