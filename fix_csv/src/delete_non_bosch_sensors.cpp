#include <cstddef>

#include <pl/algo/erase.hpp>

#include "columns.hpp"
#include "delete_non_bosch_sensors.hpp"

namespace fmc {
void deleteNonBoschSensors(std::vector<std::vector<std::string>>* data)
{
  static constexpr std::size_t zeroBaseOffset{1};

  pl::algo::erase_if(*data, [](const std::vector<std::string>& row) {
    const std::size_t index{extractIdColumn - zeroBaseOffset};

    if (row.size() <= index) { return false; }

    const std::string& extractId{row[index]};

    return (extractId != "769") && (extractId != "770") && (extractId != "771")
           && (extractId != "772");
  });
}
} // namespace fmc
