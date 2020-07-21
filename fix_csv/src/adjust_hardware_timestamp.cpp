#include <cstdio>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/current_function.hpp>

#include "cl/s2n.hpp"
#include "cl/to_string.hpp"

#include "adjust_hardware_timestamp.hpp"

namespace fmc {
void adjustHardwareTimestamp(
  std::string*   cellContent,
  std::uint64_t  overflowThreshold,
  std::uint64_t* overflowCount)
{
  const auto cellValueExpected{cl::s2n<unsigned long long>(*cellContent)};

  if (!cellValueExpected.has_value()) {
    fmt::print(
      stderr,
      "{}: s2n failed: \"{}\"\n",
      PL_CURRENT_FUNCTION,
      cellValueExpected.error());

    return;
  }

  auto cellValue{cellValueExpected.value()};

  const auto oldCellValue{cellValue};

  if (*overflowCount > 0U) {
    cellValue += *overflowCount * UINT16_MAX + *overflowCount;
  }

  if (oldCellValue >= overflowThreshold) { ++(*overflowCount); }

  *cellContent = cl::to_string(cellValue);
}
} // namespace fmc
