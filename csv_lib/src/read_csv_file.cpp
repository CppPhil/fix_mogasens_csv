#include <algorithm>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <csv.hpp>

#include "read_csv_file.hpp"
#include "unreachable.hpp"

namespace cl {
namespace {
[[nodiscard]] bool checkFirstRowSize(
  const std::vector<std::vector<std::string>>& data,
  CsvFileKind                                  csvFileKind)
{
  constexpr std::size_t rawFirstSize{11};
  constexpr std::size_t fixedFirstSize{10};

  switch (csvFileKind) {
  case CsvFileKind::Raw: return data.front().size() == rawFirstSize;
  case CsvFileKind::Fixed: return data.front().size() == fixedFirstSize;
  }

  CL_UNREACHABLE();
}

[[nodiscard]] bool checkOtherRows(
  const std::vector<std::vector<std::string>>& data,
  CsvFileKind                                  csvFileKind)
{
  constexpr std::size_t otherRowsExpectedLength{10};

  switch (csvFileKind) {
  case CsvFileKind::Raw: return true;
  case CsvFileKind::Fixed:
    return std::all_of(
      data.begin() + 1, data.end(), [](const std::vector<std::string>& row) {
        return row.size() == otherRowsExpectedLength;
      });
  }

  CL_UNREACHABLE();
}
} // namespace

[[nodiscard]] Expected<std::vector<std::vector<std::string>>> readCsvFile(
  pl::string_view           csvFilePath,
  std::vector<std::string>* columnNames,
  CsvFileKind               csvFileKind) noexcept
{
  try {
    csv::CSVReader csvReader{
      csv::string_view{csvFilePath.c_str(), csvFilePath.size()},
      csv::CSVFormat{}
        .delimiter(',')
        .quote('"')
        .header_row(0)
        .detect_bom(true)
        .variable_columns(csv::VariableColumnPolicy::KEEP)};

    std::vector<std::vector<std::string>> data(
      csvReader.begin(), csvReader.end());

    if (data.empty()) {
      return CL_UNEXPECTED(
        Error::InvalidArgument,
        fmt::format("No CSV data was read from file: \"{}\".", csvFilePath));
    }

    if (!checkFirstRowSize(data, csvFileKind)) {
      return CL_UNEXPECTED(
        Error::InvalidArgument,
        fmt::format(
          "First row of data from file \"{}\" was of unexpected length.",
          csvFilePath));
    }

    if (!checkOtherRows(data, csvFileKind)) {
      return CL_UNEXPECTED(
        Error::InvalidArgument,
        fmt::format(
          "One of the rows after the first row of data from file \"{}\" wasn't "
          "of the expected length.",
          csvFilePath));
    }

    if (csvFileKind == CsvFileKind::Raw) {
      // Remove the sampling rate entry from the first row of data.
      // This makes all the rows be uniform in length (10 columns).
      data.front().pop_back();
    }

    if (columnNames != nullptr) { *columnNames = csvReader.get_col_names(); }

    return data;
  }
  catch (const std::runtime_error& exception) {
    return CL_UNEXPECTED(Error::Filesystem, exception.what());
  }
}
} // namespace cl
