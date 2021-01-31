#ifndef INCG_FMC_ADJUST_HARDWARE_TIMESTAMP_HPP
#define INCG_FMC_ADJUST_HARDWARE_TIMESTAMP_HPP
#include <cstdint>

#include <string>

namespace fmc {
/*!
 * \brief Adjust the hardware timestamp.
 * \param cellContent Pointer to the string of a cell containing a hardware
 *                    timestamp.
 * \param nextRowHardwareTimestamp The harware timestamp in the next row.
 * \param overflowCount Pointer to the uint64_t that counts the overflows.
 **/
void adjustHardwareTimestamp(
  std::string*       cellContent,
  const std::string& nextRowHardwareTimestamp,
  std::uint64_t*     overflowCount);
} // namespace fmc
#endif // INCG_FMC_ADJUST_HARDWARE_TIMESTAMP_HPP
