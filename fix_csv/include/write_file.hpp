#ifndef INCG_FMC_WRITE_FILE_HPP
#define INCG_FMC_WRITE_FILE_HPP
#include <string>
#include <vector>

#include <pl/string_view.hpp>

namespace fmc {
[[nodiscard]] bool writeFile(
  pl::string_view                              csvPath,
  pl::string_view                              csvFileExtension,
  const std::vector<std::string>&              columnNames,
  const std::vector<std::vector<std::string>>& data);
} // namespace fmc
#endif // INCG_FMC_WRITE_FILE_HPP
