#include <algorithm>
#include <fstream>
#include <ostream>
#include <tuple>
#include <utility>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/begin_end_macro.hpp>

#include <cl/exception.hpp>
#include <cl/s2n.hpp>

#include "configuration.hpp"
#include "split_string.hpp"

namespace cm {
namespace {
/*!
 * \brief Checks if the iterator range given contains `value`.
 * \tparam Iterator The type of the iterator.
 * \tparam Ty The type of the value.
 * \param begin The start of the iterator range.
 * \param end The end iterator of the iterator range.
 * \param value The vaule to search for.
 * \return true if [begin;end) contains `value`; false otherwise.
 * \note Uses a half open range.
 **/
template<typename Iterator, typename Ty>
[[nodiscard]] bool contains(Iterator begin, Iterator end, const Ty& value)
{
  return std::any_of(begin, end, [&value](const auto& currentValue) {
    return currentValue == value;
  });
}
} // namespace

Configuration::Builder::Builder() noexcept
  : m_skipWindow{tl::nullopt}
  , m_deleteTooClose{tl::nullopt}
  , m_deleteTooLowVariance{tl::nullopt}
  , m_imu{tl::nullopt}
  , m_segmentationKind{tl::nullopt}
  , m_windowSize{tl::nullopt}
  , m_filterKind{tl::nullopt}
{
}

Configuration::Builder& Configuration::Builder::skipWindow(bool value)
{
  m_skipWindow = value;
  return *this;
}

Configuration::Builder& Configuration::Builder::deleteTooClose(bool value)
{
  m_deleteTooClose = value;
  return *this;
}

Configuration::Builder& Configuration::Builder::deleteTooLowVariance(bool value)
{
  m_deleteTooLowVariance = value;
  return *this;
}

Configuration::Builder& Configuration::Builder::imu(Imu value)
{
  m_imu = value;
  return *this;
}

Configuration::Builder& Configuration::Builder::segmentationKind(
  std::string value)
{
  m_segmentationKind = value;
  return *this;
}

Configuration::Builder& Configuration::Builder::windowSize(std::size_t value)
{
  m_windowSize = value;
  return *this;
}

Configuration::Builder& Configuration::Builder::filterKind(std::string value)
{
  m_filterKind = value;
  return *this;
}

Configuration Configuration::Builder::build() const
{
// Check is initialized.
#define CM_ENSURE_HAS_VALUE(dataMember)               \
  PL_BEGIN_MACRO                                      \
  if (!dataMember.has_value()) {                      \
    CL_THROW_FMT("\"{}\" was nullopt!", #dataMember); \
  }                                                   \
  PL_END_MACRO

  CM_ENSURE_HAS_VALUE(m_skipWindow);
  CM_ENSURE_HAS_VALUE(m_deleteTooClose);
  CM_ENSURE_HAS_VALUE(m_deleteTooLowVariance);
  CM_ENSURE_HAS_VALUE(m_imu);
  CM_ENSURE_HAS_VALUE(m_segmentationKind);
  CM_ENSURE_HAS_VALUE(m_skipWindow);
  CM_ENSURE_HAS_VALUE(m_filterKind);
#undef CM_ENSURE_HAS_VALUE

  // Check contains valid value
  const auto contains = [](const auto& container, const auto& value) {
    return ::cm::contains(container.begin(), container.end(), value);
  };

#define CM_ENSURE_CONTAINS(container, dataMember)                           \
  PL_BEGIN_MACRO                                                            \
  if (!contains(container, dataMember)) {                                   \
    CL_THROW_FMT(                                                           \
      "\"{}\" is not a valid option for \"{}\"", *dataMember, #dataMember); \
  }                                                                         \
  PL_END_MACRO

  CM_ENSURE_CONTAINS(skipWindowOptions(), m_skipWindow);
  CM_ENSURE_CONTAINS(deleteTooCloseOptions(), m_deleteTooClose);
  CM_ENSURE_CONTAINS(deleteTooLowVarianceOptions(), m_deleteTooLowVariance);
  CM_ENSURE_CONTAINS(imuOptions(), m_imu);
  CM_ENSURE_CONTAINS(segmentationKindOptions(), m_segmentationKind);
  CM_ENSURE_CONTAINS(windowSizeOptions(), m_windowSize);
  CM_ENSURE_CONTAINS(filterKindOptions(), m_filterKind);

#undef CM_ENSURE_CONTAINS
#undef CM_CONTAINS

  return Configuration{
    *m_skipWindow,
    *m_deleteTooClose,
    *m_deleteTooLowVariance,
    *m_imu,
    *m_segmentationKind,
    *m_windowSize,
    *m_filterKind};
}

const std::deque<bool>& Configuration::skipWindowOptions() noexcept
{
  static const std::deque<bool> options{false, true};
  return options;
}

const std::deque<bool>& Configuration::deleteTooCloseOptions() noexcept
{
  static const std::deque<bool> options{false, true};
  return options;
}

const std::deque<bool>& Configuration::deleteTooLowVarianceOptions() noexcept
{
  static const std::deque<bool> options{false, true};
  return options;
}

const std::vector<Imu>& Configuration::imuOptions() noexcept
{
  static const std::vector<Imu> options{Imu::Accelerometer, Imu::Gyroscope};
  return options;
}

const std::vector<std::string>&
Configuration::segmentationKindOptions() noexcept
{
  static const std::vector<std::string> values{"both", "max", "min"};
  return values;
}

const std::vector<std::size_t>& Configuration::windowSizeOptions() noexcept
{
  static const std::vector<std::size_t> values{
    101, 151, 201, 251, 301, 351, 401, 451, 501, 51, 551};
  return values;
}

const std::vector<std::string>& Configuration::filterKindOptions() noexcept
{
  static const std::vector<std::string> values{"average", "butterworth"};
  return values;
}

bool operator==(const Configuration& lhs, const Configuration& rhs) noexcept
{
  return std::tie(
           lhs.m_skipWindow,
           lhs.m_deleteTooClose,
           lhs.m_deleteTooLowVariance,
           lhs.m_imu,
           lhs.m_segmentationKind,
           lhs.m_windowSize,
           lhs.m_filterKind)
         == std::tie(
           rhs.m_skipWindow,
           rhs.m_deleteTooClose,
           rhs.m_deleteTooLowVariance,
           rhs.m_imu,
           rhs.m_segmentationKind,
           rhs.m_windowSize,
           rhs.m_filterKind);
}

bool operator!=(const Configuration& lhs, const Configuration& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const Configuration& config)
{
  return os << fmt::format(
           "Configuration{{\n"
           "\t\"skipWindow\": {},\n"
           "\t\"deleteTooClose\": {},\n"
           "\t\"deleteTooLowVariance\": {},\n"
           "\t\"imu\": \"{}\",\n"
           "\t\"segmentationKind\": \"{}\",\n"
           "\t\"windowSize\": {},\n"
           "\t\"filterKind\": \"{}\"\n"
           "}}",
           config.m_skipWindow,
           config.m_deleteTooClose,
           config.m_deleteTooLowVariance,
           config.m_imu,
           config.m_segmentationKind,
           config.m_windowSize,
           config.m_filterKind);
}

Configuration::Configuration()
  : m_skipWindow{false}
  , m_deleteTooClose{false}
  , m_deleteTooLowVariance{false}
  , m_imu{Imu::Gyroscope}
  , m_segmentationKind{"error segementation kind"}
  , m_windowSize{0}
  , m_filterKind{"error filter kind"}
  , m_isInitialized{false}
{
}

bool Configuration::skipWindow() const noexcept { return m_skipWindow; }

bool Configuration::deleteTooClose() const noexcept { return m_deleteTooClose; }

bool Configuration::deleteTooLowVariance() const noexcept
{
  return m_deleteTooLowVariance;
}

Imu Configuration::imu() const noexcept { return m_imu; }

const std::string& Configuration::segmentationKind() const noexcept
{
  return m_segmentationKind;
}

std::size_t Configuration::windowSize() const noexcept { return m_windowSize; }

const std::string& Configuration::filterKind() const noexcept
{
  return m_filterKind;
}

bool Configuration::isInitialized() const noexcept { return m_isInitialized; }

cl::fs::Path Configuration::createFilePath() const
{
  static const std::string prefix{
    "confusion_matrix/data/segmentation_points_imported_from_python/"};

  return cl::fs::Path{fmt::format(
    "{}skipWindow-{}_deleteTooClose-{}_deleteTooLowVariance-{}_imu-{}_"
    "segmentationKind-{}_windowSize-{}_filterKind-{}.txt",
    prefix,
    m_skipWindow,
    m_deleteTooClose,
    m_deleteTooLowVariance,
    m_imu,
    m_segmentationKind,
    m_windowSize,
    m_filterKind)};
}

bool Configuration::serializeSegmentationPoints(
  const std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>&
    segmentationPointsMap) const
{
  const cl::fs::Path configurationPath{createFilePath()};

  if (configurationPath.exists()) { return true; }

  std::ofstream ofs{configurationPath.str()};

  if (!ofs) { return false; }

  for (const auto& [csvFilePath, segmentationPoints] : segmentationPointsMap) {
    fmt::print(
      ofs, "{}:[{}]\n", csvFilePath, fmt::join(segmentationPoints, ","));
  }

  return static_cast<bool>(ofs);
}

std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>
Configuration::importSegmentationPoints() const
{
  // Check if the file exists.
  const cl::fs::Path configurationPath{createFilePath()};
  if (!configurationPath.exists()) {
    CL_THROW_FMT("Path \"{}\" does not exist!", configurationPath);
  }

  // Open the file.
  std::ifstream ifs{configurationPath.str()};
  if (!ifs) { CL_THROW_FMT("Could not open \"{}\"!", configurationPath); }

  std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>> result{};

  for (std::string line{}; std::getline(ifs, line);) {
    // Split the line into CSV file path and segmentation points.
    const std::vector<std::string> parts{splitString(line, ":")};
    static constexpr std::size_t   expectedPartCount{2U};
    if (parts.size() != expectedPartCount) {
      CL_THROW_FMT(
        "Splitting \"{}\" resulted in \"{}\", which is nonsense!",
        line,
        fmt::join(parts, ", "));
    }

    // Fetch the 2 parts.
    const std::string& csvFilePathString{parts.at(0)};
    std::string        segmentationPointsString{parts.at(1)};

    // Check if the CSV file path is a file.
    const cl::fs::Path csvFilePath{csvFilePathString};
    if (!csvFilePath.isFile()) {
      CL_THROW_FMT("\"{}\" is not a file!", csvFilePath);
    }

    // Check if the segmentation point string is reasonable.
    if (segmentationPointsString.size() < 2U) {
      CL_THROW_FMT("\"{}\" is too short!", segmentationPointsString);
    }
    if (segmentationPointsString.front() != '[') {
      CL_THROW_FMT("\"{}\" does not start with '['!", segmentationPointsString);
    }
    if (segmentationPointsString.back() != ']') {
      CL_THROW_FMT("\"{}\" does not end with ']'!", segmentationPointsString);
    }

    // Remove the surrounding [ & ]
    segmentationPointsString.pop_back();
    segmentationPointsString.erase(0, 1); // pop_front

    // Parse the segmentation points out of the segmentation point string.
    const std::vector<std::string> segmentationPointsAsStrings{
      splitString(segmentationPointsString, ",")};
    std::vector<std::uint64_t> segmentationPoints{};
    for (const std::string& string : segmentationPointsAsStrings) {
      const auto exp{cl::s2n<std::uint64_t>(string)};
      if (!exp.has_value()) { exp.error().raise(); }
      segmentationPoints.push_back(*exp);
    }

    result[csvFilePath] = segmentationPoints;
  }

  // We expect EOF, which sets both the eofbit and the failbit.
  // If badbit is set, that's still an error.
  if (ifs.bad()) {
    CL_THROW_FMT("ifs was bad for file \"{}\".", configurationPath);
  }

  return result;
}

Configuration::Configuration(
  bool        skipWindow,
  bool        deleteTooClose,
  bool        deleteTooLowVariance,
  Imu         imu,
  std::string segmentationKind,
  std::size_t windowSize,
  std::string filterKind)
  : m_skipWindow{skipWindow}
  , m_deleteTooClose{deleteTooClose}
  , m_deleteTooLowVariance{deleteTooLowVariance}
  , m_imu{imu}
  , m_segmentationKind{std::move(segmentationKind)}
  , m_windowSize{windowSize}
  , m_filterKind{std::move(filterKind)}
  , m_isInitialized{true}
{
}
} // namespace cm
