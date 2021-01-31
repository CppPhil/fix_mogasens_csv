#ifndef INCG_FMC_CONVERT_TO_UNIX_LINE_ENDINGS_HPP
#define INCG_FMC_CONVERT_TO_UNIX_LINE_ENDINGS_HPP
#include <string>

namespace fmc {
/*!
 * \brief Convert a CSV file to UNIX line endings.
 * \param csvPath The path to the CSV file.
 * \return true on success; false otherwise.
 **/
[[nodiscard]] bool convertToUnixLineEndings(const std::string& csvPath);
} // namespace fmc
#endif // INCG_FMC_CONVERT_TO_UNIX_LINE_ENDINGS_HPP
