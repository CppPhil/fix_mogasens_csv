#ifndef INCG_CM_CSV_FILE_INFO_HPP
#define INCG_CM_CSV_FILE_INFO_HPP
#include <cstdint>

#include <vector>

#include <cl/fs/path.hpp>

namespace cm {
/*!
 * \brief Type to hold the hardware timestamps of a CSV file.
 **/
class CsvFileInfo {
public:
  /*!
   * \brief Reads the hardware timestamps from `csvFilePath`.
   * \param csvFilePath The CSV to read the hardware timestamps from.
   * \throws cl::Exception on error.
   **/
  explicit CsvFileInfo(const cl::fs::Path& csvFilePath);

  /*!
   * \brief Read accessor for the hardware timestamps
   * \return The hardware timestamps.
   **/
  [[nodiscard]] const std::vector<std::uint64_t>& hardwareTimestamps()
    const noexcept;

private:
  std::vector<std::uint64_t> m_hardwareTimestamps;
};
} // namespace cm
#endif // INCG_CM_CSV_FILE_INFO_HPP
