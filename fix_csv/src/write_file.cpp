#include <fstream>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <csv.hpp>

#include "convert_to_unix_line_endings.hpp"
#include "write_file.hpp"

namespace fmc {
[[nodiscard]] bool writeFile(
  pl::string_view                              csvPath,
  pl::string_view                              csvFileExtension,
  const std::vector<std::string>&              columnNames,
  const std::vector<std::vector<std::string>>& data)
{
  const std::string outputFilePath{[csvPath, csvFileExtension] {
    std::string buffer(
      csvPath.c_str(), csvPath.size() - csvFileExtension.size());
    buffer += "_out";
    buffer.append(csvFileExtension.begin(), csvFileExtension.end());
    return buffer;
  }()};
  std::ofstream     outputFileStream{
    outputFilePath, std::ios_base::out | std::ios_base::trunc};

  if (!outputFileStream) {
    fmt::print(stderr, "Couldn't open \"{}\" for writing!\n", outputFilePath);
    return false;
  }

  {
    auto csvWriter = csv::make_csv_writer(outputFileStream);
    csvWriter << columnNames;

    for (const std::vector<std::string>& currentRow : data) {
      csvWriter << currentRow;
    }
  }

  outputFileStream.close();

  if (!fmc::convertToUnixLineEndings(outputFilePath)) { return false; }

  fmt::print("Successfully wrote data to \"{}\".\n", outputFilePath);

  return true;
}
} // namespace fmc
