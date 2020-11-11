#ifndef INCG_CM_MANUAL_SEGMENTATION_POINT_HPP
#define INCG_CM_MANUAL_SEGMENTATION_POINT_HPP
#include <cstdint>

#include <cl/exception.hpp>

namespace cm {
// TODO: HERE
/*  Videozeit in (Fake)-Millisekunden umrechnen.
 *  Den ersten Segmentierungspunkt von den Videos in (Fake)-Millisekunden
 * nehmen (U). Den ersten Segmentierungspunkt von Python in
 * (Real)-Millisekunden nehmen (Y). Fuer jeden weiteren Segmentierungspunkt
 * aus den Videos W die Distanz zu U berechnen. Diese Distanz (DIST U,W) auf Y
 * addieren ergiebt W in echt (Wreal = Y + DIST U,W)
 */
class ManualSegmentationPoint {
public:
  // TODO: Add a function to import the CSV file.

  /*!
   * Creates a ManualSegmentationPoint.
   *
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

  [[nodiscard]] std::uint32_t hour() const noexcept;

  [[nodiscard]] std::uint32_t minute() const noexcept;

  [[nodiscard]] std::uint32_t second() const noexcept;

  [[nodiscard]] std::uint32_t frame() const noexcept;

  [[nodiscard]] std::uint64_t asMilliseconds() const noexcept;

private:
  std::uint32_t m_hour;
  std::uint32_t m_minute;
  std::uint32_t m_second;
  std::uint32_t m_frame;
};
} // namespace cm
#endif // INCG_CM_MANUAL_SEGMENTATION_POINT_HPP
