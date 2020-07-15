#ifndef INCG_CL_COLUMN_HPP
#define INCG_CL_COLUMN_HPP
#include <cstddef>
#include <cstdint>

#include "sensor.hpp"

namespace cl {
enum class Column : std::size_t {
  Time,
  HardwareTimestamp,
  ExtractId,
  Trigger,
  AccelerometerX,
  AccelerometerY,
  AccelerometerZ,
  GyroscopeX,
  GyroscopeY,
  GyroscopeZ,
  SamplingRate
};

template<Column Col>
struct col_traits;

#define CL_SPECIALIZE_COL_TRAITS(column, columnType)                       \
  template<>                                                               \
  struct col_traits<column> {                                              \
    static constexpr std::size_t index = static_cast<std::size_t>(column); \
    using type                         = columnType;                       \
  }

CL_SPECIALIZE_COL_TRAITS(Column::Time, long double);
CL_SPECIALIZE_COL_TRAITS(Column::HardwareTimestamp, std::uint64_t);
CL_SPECIALIZE_COL_TRAITS(Column::ExtractId, Sensor);
CL_SPECIALIZE_COL_TRAITS(Column::Trigger, std::uint64_t);
CL_SPECIALIZE_COL_TRAITS(Column::AccelerometerX, long double);
CL_SPECIALIZE_COL_TRAITS(Column::AccelerometerY, long double);
CL_SPECIALIZE_COL_TRAITS(Column::AccelerometerZ, long double);
CL_SPECIALIZE_COL_TRAITS(Column::GyroscopeX, long double);
CL_SPECIALIZE_COL_TRAITS(Column::GyroscopeY, long double);
CL_SPECIALIZE_COL_TRAITS(Column::GyroscopeZ, long double);
CL_SPECIALIZE_COL_TRAITS(Column::SamplingRate, std::uint64_t);

template<Column Col>
inline constexpr std::size_t column_index = col_traits<Col>::index;

template<Column Col>
using column_type = typename col_traits<Col>::type;
} // namespace cl
#endif // INCG_CL_COLUMN_HPP
