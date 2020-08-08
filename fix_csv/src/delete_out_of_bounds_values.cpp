#include <cmath>

#include <array>
#include <stdexcept>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/algo/erase.hpp>
#include <pl/algo/ranged_algorithms.hpp>

#include "cl/channel.hpp"
#include "cl/column.hpp"
#include "cl/s2n.hpp"

#include "delete_out_of_bounds_values.hpp"

namespace fmc {
namespace {
[[nodiscard]] bool isOutOfBounds(long double value, long double threshold)
{
  return std::abs(value) >= threshold;
}

template<cl::Column Col>
inline constexpr std::size_t index_of = cl::column_index<Col>;
} // namespace

cl::Expected<void> deleteOutOfBoundsValues(
  std::vector<std::vector<std::string>>* data)
{
  static constexpr std::array accelerometerIndices{
    index_of<cl::Column::AccelerometerX>,
    index_of<cl::Column::AccelerometerY>,
    index_of<cl::Column::AccelerometerZ>};

  static constexpr std::array gyroscopeIndices{
    index_of<cl::Column::GyroscopeX>,
    index_of<cl::Column::GyroscopeY>,
    index_of<cl::Column::GyroscopeZ>};

  try {
    pl::algo::erase_if(*data, [](const std::vector<std::string>& row) {
      return pl::algo::any_of(
               accelerometerIndices,
               [&row](auto index) {
                 return isOutOfBounds(
                   cl::s2n<long double>(row.at(index)).value(),
                   cl::accelerometerThreshold);
               })
             || pl::algo::any_of(gyroscopeIndices, [&row](auto index) {
                  return isOutOfBounds(
                    cl::s2n<long double>(row.at(index)).value(),
                    cl::gyroscopeThreshold);
                });
    });
  }
  catch (const tl::bad_expected_access<cl::Error>& exception) {
    return tl::make_unexpected(exception.error());
  }
  catch (const std::out_of_range& exception) {
    return CL_UNEXPECTED(
      cl::Error::OutOfRange,
      fmt::format("IMU index was out of bounds: \"{}\"", exception.what()));
  }

  return {};
}
} // namespace fmc
