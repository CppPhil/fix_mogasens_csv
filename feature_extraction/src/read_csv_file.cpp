#include <algorithm>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <csv.hpp>

#include "read_csv_file.hpp"

namespace fe {
[[nodiscard]] Expected<std::vector<std::vector<std::string>>> readCsvFile(
  pl::string_view csvFilePath) noexcept
{
  static constexpr std::size_t zeroethRowExpectedLength{11};
  static constexpr std::size_t otherRowsExpectedLength{10};

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
      return FE_UNEXPECTED(
        Error::InvalidArgument,
        fmt::format("No CSV data was read from file: \"{}\".", csvFilePath));
    }

    if (data.front().size() != zeroethRowExpectedLength) {
      return FE_UNEXPECTED(
        Error::InvalidArgument,
        fmt::format(
          "First row of data from file \"{}\" was of length {} but length {} "
          "was expected.",
          csvFilePath,
          data.front().size(),
          zeroethRowExpectedLength));
    }

    if (std::any_of(
          data.begin() + 1,
          data.end(),
          [](const std::vector<std::string>& row) {
            return row.size() != otherRowsExpectedLength;
          })) {
      return FE_UNEXPECTED(
        Error::InvalidArgument,
        fmt::format(
          "One of the rows after the first row of data from file \"{}\" wasn't "
          "of length {}.",
          csvFilePath,
          otherRowsExpectedLength));
    }

    // Remove the sampling rate entry from the first row of data.
    // This makes all the rows be uniform in length (10 columns).
    data.front().pop_back();

    return data;
  }
  catch (const std::runtime_error& exception) {
    return FE_UNEXPECTED(Error::Filesystem, exception.what());
  }
}
} // namespace fe
