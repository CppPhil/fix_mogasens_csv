#ifndef INCG_FE_DATA_SET_HPP
#define INCG_FE_DATA_SET_HPP
#include <cstddef>

#include <string>
#include <vector>

#include "column.hpp"
#include "error.hpp"

namespace fe {
class DataSet {
public:
  using size_type = std::size_t;

  [[nodiscard]] static Expected<DataSet> create(
    const std::vector<std::vector<std::string>>& matrix);

  size_type rowCount() const noexcept;

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

private:
  explicit DataSet(
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
} // namespace fe
#endif // INCG_FE_DATA_SET_HPP
