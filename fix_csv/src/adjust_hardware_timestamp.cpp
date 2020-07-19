#include <cstdio>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/current_function.hpp>

#include "adjust_hardware_timestamp.hpp"

namespace fmc {
void adjustHardwareTimestamp(
  std::string*   cellContent,
  std::uint64_t  overflowThreshold,
  std::uint64_t* overflowCount)
{
  try {
    auto       cellValue{std::stoull(*cellContent)};
    const auto oldCellValue{cellValue};

    if (*overflowCount > 0U) {
      cellValue += *overflowCount * UINT16_MAX + *overflowCount;
      fmt::print(
        stderr,
        "of count was greater than 0 ({}), cellValue is now: {}\n",
        *overflowCount,
        cellValue);
    }

    if (oldCellValue >= overflowThreshold) {
      ++(*overflowCount);

      fmt::print(
        stderr,
        "oldCellValue {} is greater than {}, of count is now {}\n",
        oldCellValue,
        overflowThreshold,
        *overflowCount);
    }

    fmt::format_int formatter{cellValue};
    cellContent->assign(formatter.data(), formatter.size());
    fmt::print(stderr, "Set cellContent to: \"{}\"\n", *cellContent);
  }
  catch (const std::invalid_argument& ex) {
    fmt::print(
      stderr,
      "{}: stoull failed (invalid_argument): \"{}\"\n",
      PL_CURRENT_FUNCTION,
      ex.what());
  }
  catch (const std::out_of_range& ex) {
    fmt::print(
      stderr,
      "{}: stoull failed (out_of_range): \"{}\"\n",
      PL_CURRENT_FUNCTION,
      ex.what());
  }
}
} // namespace fmc
