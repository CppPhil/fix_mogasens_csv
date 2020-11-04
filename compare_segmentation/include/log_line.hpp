#ifndef INCG_CS_LOG_LINE_HPP
#define INCG_CS_LOG_LINE_HPP
#include <cstdint>

#include <string>

#include "cl/error.hpp"
#include "cl/fs/path.hpp"

namespace cs {
/*!
 * \brief A line out of a log file.
 **/
class LogLine {
public:
  /*!
   * \brief Indicates an invalid sensor.
   **/
  static const std::uint64_t invalidSensor;

  /*!
   * \brief Parses a LogLine out of a line of text read from a log file.
   * \param line The line read.
   * \return The resulting LogLine or an error.
   **/
  static cl::Expected<LogLine> parse(const std::string& line);

  /*!
   * \brief Read accessor for the segmentation point count.
   * \return The segmentation point count.
   **/
  std::uint64_t segmentationPointCount() const noexcept;

  /*!
   * \brief Read accessor for the file path.
   * \return The file path of the file in the log line.
   **/
  const cl::fs::Path& filePath() const noexcept;

  /*!
   * \brief Creates the short file name for the file in the log line.
   * \return The resulting short file name or an error.
   **/
  cl::Expected<std::string> fileName() const;

  /*!
   * \brief Read acccessor for the sensor.
   * \return The sensor.
   * \note Will only return a valid sensor if the LogLine is for a preprocessed
   *       file.
   **/
  std::uint64_t sensor() const noexcept;

private:
  LogLine(
    std::uint64_t  segmentationPointCount,
    cl::fs::Path&& filePath,
    std::uint64_t  sensor);

  std::uint64_t m_segmentationPointCount;
  cl::fs::Path  m_filePath;
  std::uint64_t m_sensor;
};
} // namespace cs
#endif // INCG_CS_LOG_LINE_HPP
