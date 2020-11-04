#ifndef INCG_CS_LOG_INFO_HPP
#define INCG_CS_LOG_INFO_HPP
#include <cstdint>

#include <iosfwd>

#include <cl/error.hpp>
#include <cl/fs/path.hpp>

#include "filter_kind.hpp"
#include "segmentation_kind.hpp"

namespace cs {
/*!
 * \brief Information about a log file.
 *
 * Information about a log file that is extracted
 * from the log file name.
 **/
class LogInfo {
public:
  /*!
   * \brief Represents an invalid sensor.
   **/
  static const std::uint64_t invalidSensor;

  /*!
   * \brief Creates a LogInfo from the given log file path.
   * \param logFilePath The log file path to create a LogInfo from.
   * \return The LogInfo created or an error.
   **/
  static cl::Expected<LogInfo> create(cl::fs::Path logFilePath) noexcept;

  /*!
   * \brief Compares two LogInfos for equality.
   * \param lhs The left hand side operand.
   * \param rhs The right hand side operand.
   * \return true if `lhs` and `rhs` are considered equal; otherwise false.
   **/
  friend bool operator==(const LogInfo& lhs, const LogInfo& rhs) noexcept;

  /*!
   * \brief Compares two LogInfos for inequality.
   * \param lhs The left hand side operand.
   * \param rhs The right hand side operand.
   * \return true if `lhs` and `rhs` are considered not equal; otherwise false.
   **/
  friend bool operator!=(const LogInfo& lhs, const LogInfo& rhs) noexcept;

  /*!
   * \brief Prints a LogInfo to an ostream.
   * \param os The ostream to print to.
   * \param logInfo The LogInfo to print.
   * \return os
   **/
  friend std::ostream& operator<<(std::ostream& os, const LogInfo& logInfo);

  /*!
   * \brief Creates an uninitialized LogInfo.
   * \warning Should only be used in order to be assigned with an
   *          initialized LogInfo; otherwise use the create static member
   *          function.
   **/
  LogInfo();

  /*!
   * \brief Read accessor for the log file path.
   * \return The log file path.
   **/
  [[nodiscard]] const cl::fs::Path& logFilePath() const noexcept;

  /*!
   * \brief Read accessor for the skip window option.
   * \return true if skip window is active; false otherwise.
   **/
  [[nodiscard]] bool skipWindow() const noexcept;

  /*!
   * \brief Read accessor for the delete too close option.
   * \return true if delete too close is active; false otherwise.
   **/
  [[nodiscard]] bool deleteTooClose() const noexcept;

  /*!
   * \brief Read accessor for the delete low variance option.
   * \return true if delete low variance is active; false otherwise.
   **/
  [[nodiscard]] bool deleteLowVariance() const noexcept;

  /*!
   * \brief Read accessor for the segmentation kind.
   * \return The segmentation kind.
   **/
  [[nodiscard]] SegmentationKind segmentationKind() const noexcept;

  /*!
   * \brief Read accessor for the window size.
   * \return The window size.
   **/
  [[nodiscard]] std::uint64_t windowSize() const noexcept;

  /*!
   * \brief Read accessor for the filter kind.
   * \return The filter kind.
   **/
  [[nodiscard]] FilterKind filterKind() const noexcept;

  /*!
   * \brief Read accessor for the sensor.
   * \return The sensor.
   * \note Will be the invalid sensor unless the log file is old.
   **/
  [[nodiscard]] std::uint64_t sensor() const noexcept;

  /*!
   * \brief Checks whether this LogInfo is initialized.
   * \return true if this LogInfo is initialized; false otherwise.
   * \note Will return true if this LogInfo was created with the create static
   *       member function.
   **/
  [[nodiscard]] bool isInitialized() const noexcept;

private:
  LogInfo(
    cl::fs::Path&&   logFilePath,
    bool             skipWindow,
    bool             deleteTooClose,
    bool             deleteLowVariance,
    SegmentationKind segmentationKind,
    std::uint64_t    windowSize,
    FilterKind       filterKind,
    std::uint64_t    sensor);

  cl::fs::Path     m_logFilePath;
  bool             m_skipWindow;
  bool             m_deleteTooClose;
  bool             m_deleteLowVariance;
  SegmentationKind m_segmentationKind;
  std::uint64_t    m_windowSize;
  FilterKind       m_filterKind;
  std::uint64_t    m_sensor;
  bool             m_isInitialized;
};
} // namespace cs
#endif // INCG_CS_LOG_INFO_HPP
