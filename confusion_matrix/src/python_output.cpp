#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/os.hpp>
#include <pl/string_view.hpp>

#include <cl/process.hpp>

#include "python_output.hpp"

namespace cm {
/*!
 * \def CM_SEGMENTOR
 * \brief Object like macro for the segmentor script.
 **/

/*!
 * \def CM_DEV_NULL
 * \brief Object like macro for /dev/null.
 **/

#if PL_OS == PL_OS_LINUX
#define CM_SEGMENTOR "./preprocessed_segment.sh"
#define CM_DEV_NULL "/dev/null"
#elif PL_OS == PL_OS_WINDOWS
#define CM_SEGMENTOR "preprocessed_segment.bat"
#define CM_DEV_NULL "NUL"
#else
#error "Unsupported operating system!"
#endif

std::string pythonOutput(
  const cl::fs::Path&  csvFilePath,
  const Configuration& segmentorConfiguration)
{
  constexpr pl::string_view readMode{"r"};

  const std::string command{fmt::format(
    CM_SEGMENTOR
    " --skip_window={} --delete_too_close={} "
    "--delete_low_variance={} --image_format=png --csv_file_path=\"{}\" "
    "--imu={} --segmentation_kind={} --window_size={} "
    "--filter={} 2>&1 >" CM_DEV_NULL,
    segmentorConfiguration.skipWindow(),
    segmentorConfiguration.deleteTooClose(),
    segmentorConfiguration.deleteTooLowVariance(),
    csvFilePath,
    segmentorConfiguration.imu(),
    segmentorConfiguration.segmentationKind(),
    segmentorConfiguration.windowSize(),
    segmentorConfiguration.filterKind())};

  cl::Expected<cl::Process> expectedProcess{
    cl::Process::create(command, readMode)};

  if (!expectedProcess.has_value()) { expectedProcess.error().raise(); }

  std::string buffer{};

  // Read
  for (int c{EOF}; (c = std::fgetc(expectedProcess->file())) != EOF;) {
    buffer.push_back(static_cast<char>(c));
  }

  return buffer;
}
} // namespace cm
