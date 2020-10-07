#ifndef INCG_CS_LOG_INFO_HPP
#define INCG_CS_LOG_INFO_HPP
#include <cstdint>

#include <cl/error.hpp>
#include <cl/fs/path.hpp>

#include "filter_kind.hpp"
#include "segmentation_kind.hpp"

namespace cs {
class LogInfo {
public:
  static cl::Expected<LogInfo> create(cl::fs::Path logFilePath) noexcept;

  [[nodiscard]] const cl::fs::Path& logFilePath() const noexcept;

  [[nodiscard]] bool skipWindow() const noexcept;

  [[nodiscard]] bool deleteTooClose() const noexcept;

  [[nodiscard]] bool deleteLowVariance() const noexcept;

  [[nodiscard]] SegmentationKind segmentationKind() const noexcept;

  [[nodiscard]] std::uint64_t windowSize() const noexcept;

  [[nodiscard]] FilterKind filterKind() const noexcept;

private:
  LogInfo(
    cl::fs::Path&&   logFilePath,
    bool             skipWindow,
    bool             deleteTooClose,
    bool             deleteLowVariance,
    SegmentationKind segmentationKind,
    std::uint64_t    windowSize,
    FilterKind       filterKind);

  cl::fs::Path     m_logFilePath;
  bool             m_skipWindow;
  bool             m_deleteTooClose;
  bool             m_deleteLowVariance;
  SegmentationKind m_segmentationKind;
  std::uint64_t    m_windowSize;
  FilterKind       m_filterKind;
};
} // namespace cs
#endif // INCG_CS_LOG_INFO_HPP
