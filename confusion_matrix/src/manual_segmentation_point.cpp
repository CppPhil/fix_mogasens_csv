#include <cmath>

#include <pl/numeric.hpp>

#include "manual_segmentation_point.hpp"

namespace cm {
ManualSegmentationPoint::ManualSegmentationPoint(
  std::uint32_t hour,
  std::uint32_t minute,
  std::uint32_t second,
  std::uint32_t frame)
  : m_hour{hour}, m_minute{minute}, m_second{second}, m_frame{frame}
{
  constexpr std::uint32_t minuteMinimum{0};
  constexpr std::uint32_t minuteMaximum{59};

  if (!pl::is_between(minute, minuteMinimum, minuteMaximum)) {
    CL_THROW_FMT(
      "Invalid minute: {} is not between {} and {}!",
      minute,
      minuteMinimum,
      minuteMaximum);
  }

  constexpr std::uint32_t secondMinimum{0};
  constexpr std::uint32_t secondMaximum{59};

  if (!pl::is_between(second, secondMinimum, secondMaximum)) {
    CL_THROW_FMT(
      "Invalid second: {} is not between {} and {}!",
      second,
      secondMinimum,
      secondMaximum);
  }

  constexpr std::uint32_t frameMinimum{0};
  constexpr std::uint32_t frameMaximum{29};

  if (!pl::is_between(frame, frameMinimum, frameMaximum)) {
    CL_THROW_FMT(
      "Invalid frame: {} is not between {} and {}!",
      frame,
      frameMinimum,
      frameMaximum);
  }
}

std::uint32_t ManualSegmentationPoint::hour() const noexcept { return m_hour; }

std::uint32_t ManualSegmentationPoint::minute() const noexcept
{
  return m_minute;
}

std::uint32_t ManualSegmentationPoint::second() const noexcept
{
  return m_second;
}

std::uint32_t ManualSegmentationPoint::frame() const noexcept
{
  return m_frame;
}

std::uint64_t ManualSegmentationPoint::asMilliseconds() const noexcept
{
  constexpr std::uint32_t framesPerSecond{30};
  constexpr std::uint32_t millisecondsPerSecond{1000};
  constexpr long double   frameTimeMilliseconds{
    static_cast<long double>(millisecondsPerSecond)
    / static_cast<long double>(framesPerSecond)};
  constexpr std::uint32_t minutesPerHour{60};
  constexpr std::uint32_t secondsPerMinute{60};

  std::uint64_t millisecondAccumulator{0};

  millisecondAccumulator
    += m_hour * minutesPerHour * secondsPerMinute * millisecondsPerSecond;
  millisecondAccumulator += m_minute * secondsPerMinute * millisecondsPerSecond;
  millisecondAccumulator += m_second * millisecondsPerSecond;
  millisecondAccumulator += static_cast<std::uint64_t>(
    std::roundl(static_cast<long double>(m_frame) * frameTimeMilliseconds));

  return millisecondAccumulator;
}
} // namespace cm
