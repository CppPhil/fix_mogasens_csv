#ifndef INCG_FMC_WRITE_FILE_HPP
#define INCG_FMC_WRITE_FILE_HPP
#include <string>
#include <vector>

#include <pl/string_view.hpp>

namespace fmc {
/*!
 * \brief Writes a 'fixed' CSV file.
 * \param csvPath Path to which to write the new 'fixed' CSV file.
 * \param csvFileExtension The file extension to use, should probably be ".csv".
 * \param columnNames The column headers.
 * \param data The matrix to write.
 * \return true on success; otherwise false.
 **/
[[nodiscard]] bool writeFile(
  pl::string_view                              csvPath,
  pl::string_view                              csvFileExtension,
  const std::vector<std::string>&              columnNames,
  const std::vector<std::vector<std::string>>& data);
} // namespace fmc
#endif // INCG_FMC_WRITE_FILE_HPP
