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
    }

    if (oldCellValue >= overflowThreshold) { ++(*overflowCount); }

    fmt::format_int formatter{cellValue};
    cellContent->assign(formatter.data(), formatter.size());
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
