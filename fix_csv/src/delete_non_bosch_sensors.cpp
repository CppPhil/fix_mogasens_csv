#include <tl/casts.hpp>

#include <pl/algo/erase.hpp>
#include <pl/string_view.hpp>
#include <pl/stringify.hpp>

#include "cl/column.hpp"
#include "cl/sensor.hpp"
#include "cl/unreachable.hpp"

#include "delete_non_bosch_sensors.hpp"

namespace fmc {
namespace {
constexpr pl::string_view extractIdString(cl::Sensor sensor) noexcept
{
  using namespace pl::string_view_literals;

  switch (sensor) {
#define CL_SENSOR_X(enm, value) \
  case cl::Sensor::enm: return PL_STRINGIFY(value);
    CL_SENSOR
#undef CL_SENSOR_X
  }

  CL_UNREACHABLE();
}
} // namespace

void deleteNonBoschSensors(std::vector<std::vector<std::string>>* data)
{
  pl::algo::erase_if(*data, [](const std::vector<std::string>& row) {
    constexpr std::size_t index{cl::column_index<cl::Column::ExtractId>};

    if (row.size() <= index) { return false; }

    const std::string& extractId{row[index]};

    return (extractId != extractIdString(cl::Sensor::LeftArm))
           && (extractId != extractIdString(cl::Sensor::Belly))
           && (extractId != extractIdString(cl::Sensor::RightArm))
           && (extractId != extractIdString(cl::Sensor::Chest));
  });
}
} // namespace fmc
