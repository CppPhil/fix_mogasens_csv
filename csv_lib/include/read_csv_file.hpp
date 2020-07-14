#ifndef INCG_FE_READ_CSV_FILE_HPP
#define INCG_FE_READ_CSV_FILE_HPP
#include <string>
#include <vector>

#include <pl/string_view.hpp>

#include "error.hpp"

namespace fe {
[[nodiscard]] Expected<std::vector<std::vector<std::string>>> readCsvFile(
  pl::string_view csvFilePath) noexcept;
} // namespace fe
#endif // INCG_FE_READ_CSV_FILE_HPP
