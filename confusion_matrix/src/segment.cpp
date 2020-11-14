#include <cstdio>
#include <cstring>

#include <string>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <cl/exception.hpp>
#include <cl/process.hpp>
#include <cl/s2n.hpp>

#include "interpolated_data_set_paths.hpp"
#include "segment.hpp"

namespace cm {
namespace {
/*!
 * \brief Runs the Python segmentor on `path`.
 * \param path The path to the CSV file to segment.
 * \return The output of the Python application.
 * \throws cl::Exception if creating the process failed.
 **/
[[nodiscard]] std::string pythonOutput(const cl::fs::Path& path)
{
  constexpr pl::string_view readMode{"r"};

  // TODO: We might want to allow for different configurations here in order
  // TODO: to be able to compare them with each other.
  const std::string command{fmt::format(
    "./preprocessed_segment.sh --skip_window=false --delete_too_close=false "
    "--delete_low_variance=false --image_format=png --csv_file_path=\"{}\" "
    "--imu=accelerometer --segmentation_kind=max --window_size=501 "
    "--filter=butterworth 2>&1 >/dev/null",
    path)};

  cl::Expected<cl::Process> expectedProcess{
    cl::Process::create(command, readMode)};

  if (!expectedProcess.has_value()) { expectedProcess.error().raise(); }

  std::string buffer{};

  for (int c{EOF}; (c = std::fgetc(expectedProcess->file())) != EOF;) {
    buffer.push_back(static_cast<char>(c));
  }

  return buffer;
}

/*!
 * \brief Splits a string by comma and space characters.
 * \param subString The string to split.
 * \return The resulting strings.
 **/
[[nodiscard]] std::vector<std::string> splitString(std::string subString)
{
  char* pch{std::strtok(subString.data(), " ,")};

  std::vector<std::string> result{};

  while (pch != nullptr) {
    result.push_back(pch);
    pch = std::strtok(nullptr, " ,");
  }

  return result;
}

/*!
 * \brief Converts a vector of strings to a vector of unsigned 64-bit integers.
 * \param vector The input vector.
 * \return The resulting integers.
 * \throws cl::Exception if converting a string to an integer fails.
 **/
[[nodiscard]] std::vector<std::uint64_t> toVecU64(
  const std::vector<std::string>& vector)
{
  std::vector<std::uint64_t> result(vector.size());

  for (std::size_t i{0}; i < vector.size(); ++i) {
    const std::string&                string{vector[i]};
    const cl::Expected<std::uint64_t> expectedInteger{
      cl::s2n<std::uint64_t>(string)};

    if (!expectedInteger.has_value()) { expectedInteger.error().raise(); }

    result[i] = *expectedInteger;
  }

  return result;
}

/*!
 * \brief Extracts the segmenting timestamps from the output of Python.
 * \param pythonResult The output of Python.
 * \return The resulting integers.
 * \throws cl::Exception if parsing fails.
 **/
[[nodiscard]] std::vector<std::uint64_t> extractTimestamps(
  const std::string& pythonResult)
{
  const auto arrayBegin{pythonResult.find_first_of('[')};
  const auto arrayEnd{pythonResult.find_last_of(']')};

  if (arrayBegin == std::string::npos) {
    CL_THROW_FMT("Couldn't find first [ in \"{}\"!", pythonResult);
  }

  if (arrayEnd == std::string::npos) {
    CL_THROW_FMT("Couldn't find last ] in \"{}\"!", pythonResult);
  }

  const std::string subString{
    pythonResult.substr(arrayBegin + 1U, arrayEnd - arrayBegin - 1U)};
  const std::vector<std::string> parts{splitString(subString)};

  return toVecU64(parts);
}
} // namespace

std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>> segment()
{
  const std::vector<cl::fs::Path> csvFiles{interpolatedDataSetPaths()};

  std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>> result{};

  for (const cl::fs::Path& path : csvFiles) {
    const std::string          pythonResult{pythonOutput(path)};
    std::vector<std::uint64_t> segmentationPoints{
      extractTimestamps(pythonResult)};

    result[path] = std::move(segmentationPoints);
  }

  return result;
}
} // namespace cm
