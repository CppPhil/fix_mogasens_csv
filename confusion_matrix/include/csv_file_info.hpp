#ifndef INCG_CM_CSV_FILE_INFO_HPP
#define INCG_CM_CSV_FILE_INFO_HPP
#include <cstdint>

#include <vector>

#include <cl/fs/path.hpp>

namespace cm {
class CsvFileInfo {
public:
  explicit CsvFileInfo(const cl::fs::Path& csvFilePath);

  [[nodiscard]] const std::vector<std::uint64_t>& hardwareTimestamps()
    const noexcept;

private:
  std::vector<std::uint64_t> m_hardwareTimestamps;
};
} // namespace cm
#endif // INCG_CM_CSV_FILE_INFO_HPP
