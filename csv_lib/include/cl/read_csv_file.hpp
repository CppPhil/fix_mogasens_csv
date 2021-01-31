#ifndef INCG_CL_READ_CSV_FILE_HPP
#define INCG_CL_READ_CSV_FILE_HPP
#include <string>
#include <vector>

#include <pl/string_view.hpp>

#include "cl/error.hpp"

namespace cl {
/*!
 * \brief Scoped enum for the CSV file kinds.
 **/
enum class CsvFileKind {
  Raw,  /*!< A raw CSV file */
  Fixed /*!< A CSV file that's been fixed by the fix_csv application */
};

/*!
 * \brief Reads a CSV file.
 * \param csvFilePath The path to the CSV file.
 * \param columnNames An output pointer to write the column names (headers)
 *                    into. Can be nullptr if the column names should not be
 *                    extracted.
 * \param csvFileKind The kind of CSV file.
 * \return The resulting matrix or an error.
 **/
[[nodiscard]] Expected<std::vector<std::vector<std::string>>> readCsvFile(
  pl::string_view           csvFilePath,
  std::vector<std::string>* columnNames = nullptr,
  CsvFileKind               csvFileKind = CsvFileKind::Fixed) noexcept;
} // namespace cl
#endif // INCG_CL_READ_CSV_FILE_HPP
