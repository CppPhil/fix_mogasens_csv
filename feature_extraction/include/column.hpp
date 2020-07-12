#ifndef INCG_FE_COLUMN_HPP
#define INCG_FE_COLUMN_HPP
#include <cstddef>
#include <cstdint>

namespace fe {
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

#define FE_SPECIALIZE_COL_TRAITS(column, columnType)                       \
  template<>                                                               \
  struct col_traits<column> {                                              \
    static constexpr std::size_t index = static_cast<std::size_t>(column); \
    using type                         = columnType;                       \
  }

FE_SPECIALIZE_COL_TRAITS(Column::Time, long double);
FE_SPECIALIZE_COL_TRAITS(Column::HardwareTimestamp, std::uint64_t);
FE_SPECIALIZE_COL_TRAITS(Column::ExtractId, std::uint64_t);
FE_SPECIALIZE_COL_TRAITS(Column::Trigger, std::uint64_t);
FE_SPECIALIZE_COL_TRAITS(Column::AccelerometerX, long double);
FE_SPECIALIZE_COL_TRAITS(Column::AccelerometerY, long double);
FE_SPECIALIZE_COL_TRAITS(Column::AccelerometerZ, long double);
FE_SPECIALIZE_COL_TRAITS(Column::GyroscopeX, long double);
FE_SPECIALIZE_COL_TRAITS(Column::GyroscopeY, long double);
FE_SPECIALIZE_COL_TRAITS(Column::GyroscopeZ, long double);
FE_SPECIALIZE_COL_TRAITS(Column::SamplingRate, std::uint64_t);

template<Column Col>
inline constexpr std::size_t column_index = col_traits<Col>::index;

template<Column Col>
using column_type = typename col_traits<Col>::type;
} // namespace fe
#endif // INCG_FE_COLUMN_HPP
