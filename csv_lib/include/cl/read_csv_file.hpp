#ifndef INCG_CL_READ_CSV_FILE_HPP
#define INCG_CL_READ_CSV_FILE_HPP
#include <string>
#include <vector>

#include <pl/string_view.hpp>

#include "cl/error.hpp"

namespace cl {
enum class CsvFileKind { Raw, Fixed };

[[nodiscard]] Expected<std::vector<std::vector<std::string>>> readCsvFile(
  pl::string_view           csvFilePath,
  std::vector<std::string>* columnNames = nullptr,
  CsvFileKind               csvFileKind = CsvFileKind::Fixed) noexcept;
} // namespace cl
#endif // INCG_CL_READ_CSV_FILE_HPP
