#include <csv.hpp>

#include <cl/exception.hpp>
#include <cl/s2n.hpp>

#include "csv_file_info.hpp"

namespace cm {

CsvFileInfo::CsvFileInfo(const cl::fs::Path& csvFilePath)
  : m_hardwareTimestamps{}
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

    constexpr std::size_t hardwareTimestampColumnIndex{1U};

    for (std::size_t rowIndex{0}; rowIndex < data.size(); ++rowIndex) {
      const std::vector<std::string>& row{data.at(rowIndex)};

      for (std::size_t columnIndex{0}; columnIndex < row.size();
           ++columnIndex) {
        const std::string& hardwareTimestampString{
          row.at(hardwareTimestampColumnIndex)};
        const cl::Expected<std::uint64_t> expectedHardwareTimestamp{
          cl::s2n<std::uint64_t>(hardwareTimestampString)};

        if (!expectedHardwareTimestamp.has_value()) {
          expectedHardwareTimestamp.error().raise();
        }

        const std::uint64_t hardwareTimestamp{
          expectedHardwareTimestamp.value()};
        m_hardwareTimestamps.push_back(hardwareTimestamp);
      }
    }
  }
  catch (const std::runtime_error& ex) {
    CL_THROW_FMT("CSVReader failed: \"{}\"", ex.what());
  }
  catch (const std::out_of_range& ex) {
    CL_THROW_FMT("std::out_of_range: \"{}\"", ex.what());
  }
}

const std::vector<std::uint64_t>& CsvFileInfo::hardwareTimestamps()
  const noexcept
{
  return m_hardwareTimestamps;
}
} // namespace cm
