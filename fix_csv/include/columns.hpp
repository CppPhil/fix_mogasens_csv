#ifndef INCG_FMC_COLUMNS_HPP
#define INCG_FMC_COLUMNS_HPP
#include "cl/column.hpp"

namespace fmc {
inline constexpr std::size_t offset{1};

inline constexpr std::size_t timeColumn{
  cl::column_index<cl::Column::Time> + offset};
inline constexpr std::size_t hardwareTimestampColumn{
  cl::column_index<cl::Column::HardwareTimestamp> + offset};
inline constexpr std::size_t extractIdColumn{
  cl::column_index<cl::Column::ExtractId> + offset};
inline constexpr std::size_t triggerColumn{
  cl::column_index<cl::Column::Trigger> + offset};
inline constexpr std::size_t accelerometerXColumn{
  cl::column_index<cl::Column::AccelerometerX> + offset}; // Channel 1
inline constexpr std::size_t accelerometerYColumn{
  cl::column_index<cl::Column::AccelerometerY> + offset}; // Channel 2
inline constexpr std::size_t accelerometerZColumn{
  cl::column_index<cl::Column::AccelerometerZ> + offset}; // Channel 3
inline constexpr std::size_t gyroscopeXColumn{
  cl::column_index<cl::Column::GyroscopeX> + offset}; // Channel 4
inline constexpr std::size_t gyroscopeYColumn{
  cl::column_index<cl::Column::GyroscopeY> + offset}; // Channel 5
inline constexpr std::size_t gyroscopeZColumn{
  cl::column_index<cl::Column::GyroscopeZ> + offset}; // Channel 6
inline constexpr std::size_t samplingRateColumn{
  cl::column_index<cl::Column::SamplingRate> + offset};
} // namespace fmc
#endif // INCG_FMC_COLUMNS_HPP
