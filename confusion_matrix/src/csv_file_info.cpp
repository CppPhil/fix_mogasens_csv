#include <csv.hpp>

#include <pl/begin_end_macro.hpp>

#include <cl/exception.hpp>
#include <cl/s2n.hpp>

#include "csv_file_info.hpp"

namespace cm {
CsvFileInfo::CsvFileInfo(const cl::fs::Path& csvFilePath)
  : m_hardwareTimestampBegin{0}
  , m_hardwareTimestampEnd{0}
  , m_hardwareTimestampStepSize{0}
{
  try {
    csv::CSVReader csvReader{
      csvFilePath.str(),
      csv::CSVFormat{}
        .delimiter(',')
        .quote('"')
        .header_row(0)
        .detect_bom(true)
        .variable_columns(csv::VariableColumnPolicy::KEEP)};

    const std::vector<std::vector<std::string>> data(
      csvReader.begin(), csvReader.end());

    constexpr std::size_t firstRowIndex{0};
    constexpr std::size_t secondRowIndex{1};
    const std::size_t     lastRowIndex{data.size() - 1U};

    // Hardware timestamp column index
    constexpr std::size_t columnIndex{1};

    const std::vector<std::string>& firstRow{data.at(firstRowIndex)};
    const std::vector<std::string>& secondRow{data.at(secondRowIndex)};
    const std::vector<std::string>& lastRow{data.at(lastRowIndex)};

    const std::string& firstHardwareTimestampString{firstRow.at(columnIndex)};
    const std::string& secondHardwareTimestampString{secondRow.at(columnIndex)};
    const std::string& lastHardwareTimestampString{lastRow.at(columnIndex)};

    const cl::Expected<std::uint64_t> firstHardwareTimestamp{
      cl::s2n<std::uint64_t>(firstHardwareTimestampString)};
    const cl::Expected<std::uint64_t> secondHardwareTimestamp{
      cl::s2n<std::uint64_t>(secondHardwareTimestampString)};
    const cl::Expected<std::uint64_t> lastHardwareTimestamp{
      cl::s2n<std::uint64_t>(lastHardwareTimestampString)};

#define CM_ENSURE_HAS_VALUE(expected)                      \
  PL_BEGIN_MACRO                                           \
  if (!expected.has_value()) { expected.error().raise(); } \
  PL_END_MACRO

    CM_ENSURE_HAS_VALUE(firstHardwareTimestamp);
    CM_ENSURE_HAS_VALUE(secondHardwareTimestamp);
    CM_ENSURE_HAS_VALUE(lastHardwareTimestamp);

#undef CM_ENSURE_HAS_VALUE

    m_hardwareTimestampBegin = *firstHardwareTimestamp;
    m_hardwareTimestampEnd   = *lastHardwareTimestamp;
    m_hardwareTimestampStepSize
      = *secondHardwareTimestamp - *firstHardwareTimestamp;
  }
  catch (const std::runtime_error& ex) {
    CL_THROW_FMT("CSVReader failed: \"{}\"", ex.what());
  }
  catch (const std::out_of_range& ex) {
    CL_THROW_FMT("std::out_of_range: \"{}\"", ex.what());
  }
}

std::uint64_t CsvFileInfo::hardwareTimestampBegin() const noexcept
{
  return m_hardwareTimestampBegin;
}

std::uint64_t CsvFileInfo::hardwareTimestampEnd() const noexcept
{
  return m_hardwareTimestampEnd;
}

std::uint64_t CsvFileInfo::hardwareTimestampStepSize() const noexcept
{
  return m_hardwareTimestampStepSize;
}
} // namespace cm
