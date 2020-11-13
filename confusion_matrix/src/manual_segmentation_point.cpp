#include <cassert>
#include <cmath>

#include <csv.hpp>

#include <pl/numeric.hpp>

#include "cl/fs/path.hpp"
#include "cl/s2n.hpp"

#include "manual_segmentation_point.hpp"

// TODO: Write tests for the CSV file parsing.
namespace cm {
namespace {
/*!
 * \brief Converts hours, minutes or seconds from a time string
 *        such as "00:00:09"
 * \param time The time string to parse a time component out of.
 * \param startIndex The (0 based) index that the time component (hour, minute
 *                   or second) starts at.
 * \return The time component extracted as an integer.
 * \throws cl::Exception if parsing fails.
 **/
[[nodiscard]] std::uint32_t parseTimeComponent(
  const std::string& time,
  std::size_t        startIndex)
{
  // Buffer of 2 bytes for the two characters of the time component.
  std::string buffer(2U, ' ');

  try {
    // Extract the two characters representing the time component.
    buffer[0] = time.at(startIndex);
    buffer[1] = time.at(startIndex + 1);

    // Convert the string to an integer.
    const cl::Expected<std::uint32_t> expected{cl::s2n<std::uint32_t>(buffer)};

    if (!expected.has_value()) { expected.error().raise(); }

    return *expected;
  }
  catch (const std::out_of_range& ex) {
    CL_THROW(ex.what());
  }
}

/*!
 * \brief A time point comprised of hour, minute and second values.
 **/
struct TimePoint {
  /*!
   * \brief Creates a `TimePoint` struct from a time string such as "00:00:09".
   * \param time The time string to parse.
   * \return The resulting `TimePoint` struct.
   * \throws cl::Exception if parsing `time` fails.
   **/
  static TimePoint parse(const std::string& time)
  {
    constexpr std::size_t hourStartIndex{0};
    constexpr std::size_t minuteStartIndex{3};
    constexpr std::size_t secondStartIndex{6};
    return TimePoint{
      parseTimeComponent(time, hourStartIndex),
      parseTimeComponent(time, minuteStartIndex),
      parseTimeComponent(time, secondStartIndex)};
  }

  std::uint32_t hour;   /*!< The hour */
  std::uint32_t minute; /*!< The minute */
  std::uint32_t second; /*!< The second */
};

/*!
 * \brief Parses a `ManualSegmentationPoint` from a row from the CSV file.
 * \param row The row to parse.
 * \param timeIndex Index at which a time string is to be found in `row`.
 * \param frameIndex Index at which the corresponding frame string is to be
 *        found in `row`.
 * \return The `ManualSegmentationPoint` parsed out of the `row`.
 * \note `frameIndex` shall be 1 larger than `timeIndex`.
 * \throws cl::Exception if parsing fails.
 **/
[[nodiscard]] ManualSegmentationPoint parse(
  const std::vector<std::string>& row,
  std::size_t                     timeIndex,
  std::size_t                     frameIndex)
{
  assert(
    (frameIndex == (timeIndex + 1U))
    && "frameIndex must be 1 larger than the timeIndex!");

  try {
    // Fetch the strings.
    const std::string& timeString{row.at(timeIndex)};
    const std::string& frameString{row.at(frameIndex)};

    // Parse the TimePoint.
    const TimePoint timePoint{TimePoint::parse(timeString)};

    // Parse the frame.
    const cl::Expected<std::uint32_t> expectedFrame{
      cl::s2n<std::uint32_t>(frameString)};

    if (!expectedFrame.has_value()) { expectedFrame.error().raise(); }

    const std::uint32_t frame{*expectedFrame};

    return ManualSegmentationPoint{
      /* hour */ timePoint.hour,
      /* minute */ timePoint.minute,
      /* second */ timePoint.second,
      /* frame */ frame};
  }
  catch (const std::out_of_range& ex) {
    CL_THROW_FMT(
      "Out of bounds access (timeIndex: {}, frameIndex: {}): \"{}\"!\nRow: {}",
      timeIndex,
      frameIndex,
      ex.what(),
      fmt::join(row, ", "));
  }
}
} // namespace

std::unordered_map<DataSetIdentifier, std::vector<ManualSegmentationPoint>>
ManualSegmentationPoint::readCsvFile()
{
  const cl::fs::Path csvFilePath{
    "confusion_matrix/data/manual_segmentation.csv"};

  if (!csvFilePath.isFile()) {
    CL_THROW_FMT("\"{}\" is not a file!", csvFilePath);
  }

  std::unordered_map<DataSetIdentifier, std::vector<ManualSegmentationPoint>>
    result{};

  try {
    csv::CSVReader csvReader{
      csvFilePath.str(),
      csv::CSVFormat{}
        .delimiter(',')
        .quote('"')
        .header_row(0)
        .detect_bom(true)
        .variable_columns(csv::VariableColumnPolicy::KEEP)};

    const std::vector<std::vector<std::string>> matrix(
      csvReader.begin(), csvReader.end());

    constexpr std::size_t felix_11_17_39_timeColumn{1};
    constexpr std::size_t felix_11_17_39_frameColumn{2};
    constexpr std::size_t felix_12_50_00_timeColumn{5};
    constexpr std::size_t felix_12_50_00_frameColumn{6};
    constexpr std::size_t felix_13_00_09_timeColumn{9};
    constexpr std::size_t felix_13_00_09_frameColumn{10};
    constexpr std::size_t mike_14_07_33_timeColumn{13};
    constexpr std::size_t mike_14_07_33_frameColumn{14};
    constexpr std::size_t mike_14_14_32_timeColumn{17};
    constexpr std::size_t mike_14_14_32_frameColumn{18};
    constexpr std::size_t mike_14_20_28_timeColumn{21};
    constexpr std::size_t mike_14_20_28_frameColumn{22};
    constexpr std::size_t marsi_14_59_59_timeColumn{25};
    constexpr std::size_t marsi_14_59_59_frameColumn{26};
    constexpr std::size_t marsi_15_13_22_timeColumn{29};
    constexpr std::size_t marsi_15_13_22_frameColumn{30};
    constexpr std::size_t marsi_15_31_36_timeColumn{33};
    constexpr std::size_t marsi_15_31_36_frameColumn{34};
    constexpr std::size_t jan_1_timeColumn{37};
    constexpr std::size_t jan_1_frameColumn{38};
    constexpr std::size_t jan_2_timeColumn{41};
    constexpr std::size_t jan_2_frameColumn{42};
    constexpr std::size_t jan_3_timeColumn{45};
    constexpr std::size_t jan_3_frameColumn{46};
    constexpr std::size_t andre_1_timeColumn{49};
    constexpr std::size_t andre_1_frameColumn{50};
    constexpr std::size_t andre_2_timeColumn{53};
    constexpr std::size_t andre_2_frameColumn{54};
    constexpr std::size_t andre_3_timeColumn{57};
    constexpr std::size_t andre_3_frameColumn{58};
    constexpr std::size_t andre_squats_1_timeColumn{61};
    constexpr std::size_t andre_squats_1_frameColumn{62};
    constexpr std::size_t andre_squats_2_timeColumn{65};
    constexpr std::size_t andre_squats_2_frameColumn{66};
    constexpr std::size_t lucas_1_timeColumn{69};
    constexpr std::size_t lucas_1_frameColumn{70};
    constexpr std::size_t lucas_2_timeColumn{73};
    constexpr std::size_t lucas_2_frameColumn{74};
    constexpr std::size_t lucas_3_timeColumn{77};
    constexpr std::size_t lucas_3_frameColumn{78};

#define DSI DataSetIdentifier

    for (const std::vector<std::string>& row : matrix) {
      const auto append // Lambda to append.
        = [&result, &row](
            DataSetIdentifier dsi, std::size_t timeCol, std::size_t frameCol) {
            // Skip empty ones.
            if (row.at(timeCol).empty() && row.at(frameCol).empty()) { return; }

            result[dsi].push_back(parse(row, timeCol, frameCol));
          };

      append(
        DSI::Felix_11_17_39,
        felix_11_17_39_timeColumn,
        felix_11_17_39_frameColumn);
      append(
        DSI::Felix_12_50_00,
        felix_12_50_00_timeColumn,
        felix_12_50_00_frameColumn);
      append(
        DSI::Felix_13_00_09,
        felix_13_00_09_timeColumn,
        felix_13_00_09_frameColumn);
      append(
        DSI::Mike_14_07_33,
        mike_14_07_33_timeColumn,
        mike_14_07_33_frameColumn);
      append(
        DSI::Mike_14_14_32,
        mike_14_14_32_timeColumn,
        mike_14_14_32_frameColumn);
      append(
        DSI::Mike_14_20_28,
        mike_14_20_28_timeColumn,
        mike_14_20_28_frameColumn);
      append(
        DSI::Marsi_14_59_59,
        marsi_14_59_59_timeColumn,
        marsi_14_59_59_frameColumn);
      append(
        DSI::Marsi_15_13_22,
        marsi_15_13_22_timeColumn,
        marsi_15_13_22_frameColumn);
      append(
        DSI::Marsi_15_31_36,
        marsi_15_31_36_timeColumn,
        marsi_15_31_36_frameColumn);
      append(DSI::Jan_1, jan_1_timeColumn, jan_1_frameColumn);
      append(DSI::Jan_2, jan_2_timeColumn, jan_2_frameColumn);
      append(DSI::Jan_3, jan_3_timeColumn, jan_3_frameColumn);
      append(DSI::Andre_1, andre_1_timeColumn, andre_1_frameColumn);
      append(DSI::Andre_2, andre_2_timeColumn, andre_2_frameColumn);
      append(DSI::Andre_3, andre_3_timeColumn, andre_3_frameColumn);
      append(
        DSI::Andre_Squats_1,
        andre_squats_1_timeColumn,
        andre_squats_1_frameColumn);
      append(
        DSI::Andre_Squats_2,
        andre_squats_2_timeColumn,
        andre_squats_2_frameColumn);
      append(DSI::Lucas_1, lucas_1_timeColumn, lucas_1_frameColumn);
      append(DSI::Lucas_2, lucas_2_timeColumn, lucas_2_frameColumn);
      append(DSI::Lucas_3, lucas_3_timeColumn, lucas_3_frameColumn);
    }
  }
  catch (const std::runtime_error& ex) {
    CL_THROW_FMT(
      "Failure reading CSV file \"{}\": \"{}\"", csvFilePath, ex.what());
  }
#undef DSI

  return result;
}

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
