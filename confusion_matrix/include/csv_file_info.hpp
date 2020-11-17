#ifndef INCG_CM_CSV_FILE_INFO_HPP
#define INCG_CM_CSV_FILE_INFO_HPP
#include <cstdint>

#include <cl/fs/path.hpp>

namespace cm {
class CsvFileInfo {
public:
  explicit CsvFileInfo(const cl::fs::Path& csvFilePath);

  [[nodiscard]] std::uint64_t hardwareTimestampBegin() const noexcept;

  [[nodiscard]] std::uint64_t hardwareTimestampEnd() const noexcept;

  [[nodiscard]] std::uint64_t hardwareTimestampStepSize() const noexcept;

private:
  std::uint64_t m_hardwareTimestampBegin;
  std::uint64_t m_hardwareTimestampEnd;
  std::uint64_t m_hardwareTimestampStepSize;
};
} // namespace cm
#endif // INCG_CM_CSV_FILE_INFO_HPP
