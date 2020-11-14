#include <cstdio>

#include <string>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <cl/process.hpp>

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
} // namespace

std::unordered_map<DataSetIdentifier, std::vector<std::uint64_t>> segment()
{
  const std::vector<cl::fs::Path> csvFiles{interpolatedDataSetPaths()};

  for (const cl::fs::Path& path : csvFiles) {
    const std::string pythonResult{pythonOutput(path)};

    fmt::print("path: \"{}\", pythonResult: \"{}\"\n", path, pythonResult);
  }

  // TODO: THIS HAS TO CHANGE
  return {};
}
} // namespace cm
