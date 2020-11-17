#ifndef INCG_CM_MANUAL_SEGMENTATION_POINT_HPP
#define INCG_CM_MANUAL_SEGMENTATION_POINT_HPP
#include <cstdint>

#include <iosfwd>
#include <unordered_map>
#include <vector>

#include <cl/exception.hpp>

#include "data_set_identifier.hpp"

namespace cm {
/*!
 * \brief Type used to represent a manual segmentation point.
 **/
class ManualSegmentationPoint {
public:
  /*!
   * \brief Compares two manual segmentation points for equality.
   * \param lhs The first operand.
   * \param rhs The second operand.
   * \return true if `lhs` is considered equal to `rhs`; false otherwise.
   **/
  friend bool operator==(
    const ManualSegmentationPoint& lhs,
    const ManualSegmentationPoint& rhs) noexcept;

  /*!
   * \brief Compares two manual segmentation points for inequality.
   * \param lhs The first operand.
   * \param rhs The second operand.
   * \return true if `lhs` is considered not equal to `rhs`; false otherwise.
   **/
  friend bool operator!=(
    const ManualSegmentationPoint& lhs,
    const ManualSegmentationPoint& rhs) noexcept;

  /*!
   * \brief Prints `manualSegmentationPoint` to `os`.
   * \param os The ostream to print to
   * \param manualSegmentationPoint The `ManualSegmentationPoint` to print.
   * \return `os`
   **/
  friend std::ostream& operator<<(
    std::ostream&                  os,
    const ManualSegmentationPoint& manualSegmentationPoint);

  /*!
   * \brief Reads the CSV file of the manual segmentation points.
   * \return A map that maps the `DataSetIdentifier` enumerators to vectors of
   *         the corresponding manual segmentation points extracted from the CSV
   *         file.
   * \throws cl::Exception if parsing fails, CSV processing fails or the CSV
   *                       file is missing.
   **/
  static std::
    unordered_map<DataSetIdentifier, std::vector<ManualSegmentationPoint>>
    readCsvFile();

  /*!
   * \brief Converts `manualSegmentationPoints` imported from the CSV file
   *        to hardware timestamps.
   * \param manualSegmentationPoints The manual segmentation points that
   *                                 were read from the CSV file.
   * \param pythonResult The result from Python of a (good) `Configuration`
   *                     to use for the first segmentation point in order
   *                     to convert the manual segmentation points to
   *                     ones that are based on hardware timestamps.
   * \return The resulting hardware timestamp based manual segementation points.
   * \throws cl::Exception on error.
   **/
  static std::unordered_map<DataSetIdentifier, std::vector<std::uint64_t>>
  convertToHardwareTimestamps(
    const std::unordered_map<
      DataSetIdentifier,
      std::vector<ManualSegmentationPoint>>& manualSegmentationPoints,
    const std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>&
      pythonResult);

  /*!
   * \brief Creates a ManualSegmentationPoint.
   * \param hour The hour to use. Must be within [0,59].
   * \param minute The minute to use. Must be within [0,59].
   * \param second The second to use. Must be within [0,59].
   * \param frame The frame to use. Must be within [0,29].
   * \throws cl::Exception if one of the arguments is out of bounds.
   **/
  ManualSegmentationPoint(
    std::uint32_t hour,
    std::uint32_t minute,
    std::uint32_t second,
    std::uint32_t frame);

  /*!
   * \brief Read accessor for the hour property.
   * \return The hour.
   **/
  [[nodiscard]] std::uint32_t hour() const noexcept;

  /*!
   * \brief Read accessor for the minute property.
   * \return The minute.
   **/
  [[nodiscard]] std::uint32_t minute() const noexcept;

  /*!
   * \brief Read accessor for the second property.
   * \return The second.
   **/
  [[nodiscard]] std::uint32_t second() const noexcept;

  /*!
   * \brief Read accessor for the frame property.
   * \return The frame within the second of this manual segmentation point.
   **/
  [[nodiscard]] std::uint32_t frame() const noexcept;

  /*!
   * \brief Converts this manual segmentation point into a millisecond
   *        representation.
   * \return This manual segmentation point converted to milliseconds.
   **/
  [[nodiscard]] std::uint64_t asMilliseconds() const noexcept;

private:
  std::uint32_t m_hour;
  std::uint32_t m_minute;
  std::uint32_t m_second;
  std::uint32_t m_frame;
};
} // namespace cm
#endif // INCG_CM_MANUAL_SEGMENTATION_POINT_HPP
