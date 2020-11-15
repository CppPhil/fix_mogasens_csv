#include <cl/exception.hpp>
#include <cl/s2n.hpp>

#include "interpolated_data_set_paths.hpp"
#include "python_output.hpp"
#include "segment.hpp"
#include "split_string.hpp"

namespace cm {
namespace {
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
  constexpr pl::string_view      splitBy{" ,"};
  const std::vector<std::string> parts{splitString(subString, splitBy)};

  return toVecU64(parts);
}
} // namespace

// TODO: map<Config, map<path, segmentationPoints>>
std::unordered_map<
  Configuration,
  std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>
segment()
{
  const std::vector<cl::fs::Path> csvFiles{interpolatedDataSetPaths()};

  std::unordered_map<
    Configuration,
    std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>
    result{};

  for (bool skipWindowOption : Configuration::skipWindowOptions()) {
    for (bool deleteTooCloseOption : Configuration::deleteTooCloseOptions()) {
      for (bool deleteTooLowVarianceOption :
           Configuration::deleteTooLowVarianceOptions()) {
        for (Imu imuOption : Configuration::imuOptions()) {
          for (const std::string& segmentationKindOption :
               Configuration::segmentationKindOptions()) {
            for (std::size_t windowSizeOption :
                 Configuration::windowSizeOptions()) {
              for (const std::string& filterKindOption :
                   Configuration::filterKindOptions()) {
                const Configuration configuration{
                  Configuration::Builder{}
                    .skipWindow(skipWindowOption)
                    .deleteTooClose(deleteTooCloseOption)
                    .deleteTooLowVariance(deleteTooLowVarianceOption)
                    .imu(imuOption)
                    .segmentationKind(segmentationKindOption)
                    .windowSize(windowSizeOption)
                    .filterKind(filterKindOption)
                    .build()};

                result[configuration] = std::
                  unordered_map<cl::fs::Path, std::vector<std::uint64_t>>{

                  };

                for (const cl::fs::Path& csvFile : csvFiles) {
                  const std::string pythonResult{
                    pythonOutput(csvFile, configuration)};
                  std::vector<std::uint64_t> segmentationPoints{
                    extractTimestamps(pythonResult)};
                  result[configuration][csvFile]
                    = std::move(segmentationPoints);
                }
              }
            }
          }
        }
      }
    }
  }

  return result;
}
} // namespace cm
