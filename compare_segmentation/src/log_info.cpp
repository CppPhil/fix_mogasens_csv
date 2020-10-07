#include <cstddef>

#include <regex>
#include <utility>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/string_view.hpp>

#include "cl/s2n.hpp"

#include "log_info.hpp"
#include "paths.hpp"

namespace cs {
namespace {
[[nodiscard]] cl::Expected<bool> parseBool(const std::csub_match& csubMatch)
{
  if (csubMatch.compare("true") == 0) { return true; }

  if (csubMatch.compare("false") == 0) { return false; }

  return CL_UNEXPECTED(
    cl::Error::Parsing,
    fmt::format("\"{}\" could not be parsed as bool!", csubMatch.str()));
}
} // namespace

cl::Expected<LogInfo> LogInfo::create(cl::fs::Path logFilePath) noexcept
{
  // + 1 for the directory separator (/ or \)
  const std::size_t minimumSize{logPath.size() + 1};

  pl::string_view pathStringView{logFilePath.str()};

  if (pathStringView.size() <= minimumSize) {
    return CL_UNEXPECTED(
      cl::Error::InvalidArgument,
      fmt::format("\"{}\" is too short to be valid!", pathStringView));
  }

  pathStringView.remove_prefix(minimumSize);

  static constexpr char regularExpressionString[]
    = R"(^skip_window-([[:alpha:]]{4,5})_delete_too_close-([[:alpha:]]{4,5})_delete_low_variance-([[:alpha:]]{4,5})_kind-([[:alpha:]]{3,4})_window-(\d{2,3})_filter-([[:alpha:]]{7,11})\.log$)";
  constexpr std::size_t   submatches{6};
  constexpr std::size_t   expectedCmatchSize{submatches + 1};
  constexpr std::size_t   skipWindowIndex{1};
  constexpr std::size_t   deleteTooCloseIndex{2};
  constexpr std::size_t   deleteLowVarianceIndex{3};
  constexpr std::size_t   segmentationKindIndex{4};
  constexpr std::size_t   windowSizeIndex{5};
  constexpr std::size_t   filterIndex{6};
  static const std::regex regularExpression{regularExpressionString};

  std::cmatch cmatch{};

  const bool matched{
    std::regex_match(pathStringView.c_str(), cmatch, regularExpression)};

  if (!matched) {
    return CL_UNEXPECTED(
      cl::Error::InvalidArgument,
      fmt::format(
        "\"{}\" did not match the regex \"{}\"!",
        pathStringView,
        regularExpressionString));
  }

  if (cmatch.size() != expectedCmatchSize) {
    return CL_UNEXPECTED(
      cl::Error::Parsing,
      fmt::format(
        "\"{}\" produced a cmatch of size {} for regex \"{}\" but a size of {} "
        "was expected!",
        pathStringView,
        cmatch.size(),
        regularExpressionString,
        expectedCmatchSize));
  }

  const std::csub_match skipWindowCsubMatch{cmatch[skipWindowIndex]};
  const std::csub_match deleteTooCloseCsubMatch{cmatch[deleteTooCloseIndex]};
  const std::csub_match deleteLowVarianceCsubMatch{
    cmatch[deleteLowVarianceIndex]};
  const std::csub_match segmentationKindCsubMatch{
    cmatch[segmentationKindIndex]};
  const std::csub_match windowSizeCsubMatch{cmatch[windowSizeIndex]};
  const std::csub_match filterCsubMatch{cmatch[filterIndex]};

  bool             skipWindowValue, deleteTooCloseValue, deleteLowVarianceValue;
  SegmentationKind segmentationKindValue;
  std::uint64_t    windowSizeValue;
  FilterKind       filterKindValue;

#define CS_PARSE_BOOL(submatch, output)        \
  do {                                         \
    auto res = parseBool(submatch);            \
    if (res.has_value()) {                     \
      output = res.value();                    \
      return tl::make_unexpected(res.error()); \
    }                                          \
  } while ((void)0, 0)

  CS_PARSE_BOOL(skipWindowCsubMatch, skipWindowValue);
  CS_PARSE_BOOL(deleteTooCloseCsubMatch, deleteTooCloseValue);
  CS_PARSE_BOOL(deleteLowVarianceCsubMatch, deleteLowVarianceValue);

#undef CS_PARSE_BOOL

  if (segmentationKindCsubMatch.compare("min")) {
    segmentationKindValue = SegmentationKind::Minima;
  }
  else if (segmentationKindCsubMatch.compare("max")) {
    segmentationKindValue = SegmentationKind::Maxima;
  }
  else if (segmentationKindCsubMatch.compare("both")) {
    segmentationKindValue = SegmentationKind::Both;
  }
  else {
    return CL_UNEXPECTED(
      cl::Error::Parsing,
      fmt::format(
        "SegmentationKind \"{}\" is not valid!",
        segmentationKindCsubMatch.str()));
  }

  const cl::Expected<std::uint64_t> parsedWindowSize{
    cl::s2n<std::uint64_t>(windowSizeCsubMatch.str())};

  if (!parsedWindowSize.has_value()) {
    return tl::make_unexpected(parsedWindowSize.error());
  }

  windowSizeValue = parsedWindowSize.value();

  if (filterCsubMatch.compare("average")) {
    filterKindValue = FilterKind::MovingAverage;
  }
  else if (filterCsubMatch.compare("butterworth")) {
    filterKindValue = FilterKind::Butterworth;
  }
  else {
    return CL_UNEXPECTED(
      cl::Error::Parsing,
      fmt::format("\"{}\" is not a valid filter kind!", filterCsubMatch.str()));
  }

  return LogInfo{
    std::move(logFilePath),
    skipWindowValue,
    deleteTooCloseValue,
    deleteLowVarianceValue,
    segmentationKindValue,
    windowSizeValue,
    filterKindValue};
}

const cl::fs::Path& LogInfo::logFilePath() const noexcept
{
  return m_logFilePath;
}

bool LogInfo::skipWindow() const noexcept { return m_skipWindow; }

bool LogInfo::deleteTooClose() const noexcept { return m_deleteTooClose; }

bool LogInfo::deleteLowVariance() const noexcept { return m_deleteLowVariance; }

SegmentationKind LogInfo::segmentationKind() const noexcept
{
  return m_segmentationKind;
}

std::uint64_t LogInfo::windowSize() const noexcept { return m_windowSize; }

FilterKind LogInfo::filterKind() const noexcept { return m_filterKind; }

LogInfo::LogInfo(
  cl::fs::Path&&   logFilePath,
  bool             skipWindow,
  bool             deleteTooClose,
  bool             deleteLowVariance,
  SegmentationKind segmentationKind,
  std::uint64_t    windowSize,
  FilterKind       filterKind)
  : m_logFilePath{std::move(logFilePath)}
  , m_skipWindow{skipWindow}
  , m_deleteTooClose{deleteTooClose}
  , m_deleteLowVariance{deleteLowVariance}
  , m_segmentationKind{segmentationKind}
  , m_windowSize{windowSize}
  , m_filterKind{filterKind}
{
}
} // namespace cs
