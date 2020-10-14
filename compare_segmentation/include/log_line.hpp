#ifndef INCG_CS_LOG_LINE_HPP
#define INCG_CS_LOG_LINE_HPP
#include <cstdint>

#include <string>

#include "cl/error.hpp"
#include "cl/fs/path.hpp"

namespace cs {
class LogLine {
public:
  static const std::uint64_t invalidSensor;

  static cl::Expected<LogLine> parse(const std::string& line);

  std::uint64_t segmentationPointCount() const noexcept;

  const cl::fs::Path& filePath() const noexcept;

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
