#ifndef INCG_FMC_ADJUST_HARDWARE_TIMESTAMP_HPP
#define INCG_FMC_ADJUST_HARDWARE_TIMESTAMP_HPP
#include <cstdint>

#include <string>

namespace fmc {
void adjustHardwareTimestamp(
  std::string*   cellContent,
  std::uint64_t  overflowThreshold,
  std::uint64_t* overflowCount);
} // namespace fmc
#endif // INCG_FMC_ADJUST_HARDWARE_TIMESTAMP_HPP
