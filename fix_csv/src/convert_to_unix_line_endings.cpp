#include <cstdio>

#include <vector>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/byte.hpp>

#include "cl/dos2unix.hpp"
#include "cl/error.hpp"
#include "cl/fs/file.hpp"
#include "cl/fs/file_stream.hpp"

#include "convert_to_unix_line_endings.hpp"

namespace fmc {
bool convertToUnixLineEndings(const std::string& csvPath)
{
  cl::fs::File csvFile{cl::fs::Path{csvPath}};
  std::vector<pl::byte>
    crlfCsvData{}; // The CR LF line terminated data will go here.
  {
    cl::Expected<cl::fs::FileStream> fileStream{
      cl::fs::FileStream::create(csvFile, cl::fs::FileStream::Read)};
    if (!fileStream.has_value()) {
      fmt::print(
        stderr,
        "Couldn't open file stream for reading to file \"{}\"!\n",
        csvPath);
      return false;
    }
    crlfCsvData = fileStream->readAll();
  }
  // Convert to Unix line endings.
  const std::vector<pl::byte> lfCsvData{
    cl::dos2unix(crlfCsvData.data(), crlfCsvData.size())};
  if (!csvFile.remove()) {
    fmt::print(stderr, "Couldn't delete file \"{}\"!\n", csvPath);
    return false;
  }
  if (!csvFile.create()) {
    fmt::print(
      stderr,
      "Couldn't create file \"{}\" after having deleted it!\n",
      csvPath);
    return false;
  }
  cl::Expected<cl::fs::FileStream> filestream{
    cl::fs::FileStream::create(csvFile, cl::fs::FileStream::Write)};
  if (!filestream.has_value()) {
    fmt::print(
      stderr,
      "Couldn't open file stream for writing for file \"{}\"!\n",
      csvPath);
    return false;
  }
  // Write the Unix line endings back to the file.
  if (!filestream->write(lfCsvData.data(), lfCsvData.size())) {
    fmt::print(stderr, "Couldn't write LF data to file \"{}\"!\n", csvPath);
    return false;
  }
  return true;
}
} // namespace fmc
