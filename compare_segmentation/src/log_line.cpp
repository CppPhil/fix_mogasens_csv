#include <cstddef>

#include <regex>
#include <string>
#include <utility>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/strcontains.hpp>

#include "cl/s2n.hpp"

#include "log_line.hpp"

namespace cs {
namespace {
[[nodiscard]] bool isOldPath(const std::string& line)
{
  return !pl::strcontains(line, "preprocessed");
}
} // namespace

const std::uint64_t LogLine::invalidSensor = UINT64_C(0xFFFFFFFFFFFFFFFF);

cl::Expected<LogLine> LogLine::parse(const std::string& line)
{
  const bool isOld{isOldPath(line)};

  static constexpr char regularExpressionString[]
    = R"~(^.*\.py:\s(\d+)\ssegmentation\spoints\sfound\sin\s"(resources/[[:graph:]]+\.csv)"\.$)~";

  constexpr std::size_t captureGroupCount{2};
  constexpr std::size_t expectedSmatchSize{captureGroupCount + 1};
  constexpr std::size_t segmentationPointCountIndex{1};
  constexpr std::size_t filePathIndex{2};

  static const std::regex regex{regularExpressionString};

  std::smatch smatch{};

  const bool matched{std::regex_match(line, smatch, regex)};

  if (!matched) {
    return CL_UNEXPECTED(
      cl::Error::InvalidArgument,
      fmt::format(
        "\"{}\" did not match the regex \"{}\"!",
        line,
        regularExpressionString));
  }

  if (smatch.size() != expectedSmatchSize) {
    return CL_UNEXPECTED(
      cl::Error::InvalidArgument,
      fmt::format(
        "smatch was of size {}, but size {} was expected. line: \"{}\", regex: "
        "\"{}\".",
        smatch.size(),
        expectedSmatchSize,
        line,
        regularExpressionString));
  }

  const std::string segmentationPointCountString{
    smatch[segmentationPointCountIndex].str()};
  const std::string filePathString{smatch[filePathIndex].str()};

  const cl::Expected<std::uint64_t> expectedSegmentationPointCountValue{
    cl::s2n<std::uint64_t>(segmentationPointCountString)};

  if (!expectedSegmentationPointCountValue.has_value()) {
    return tl::make_unexpected(expectedSegmentationPointCountValue.error());
  }

  const std::uint64_t segmentationPointCountValue{
    *expectedSegmentationPointCountValue};

  const auto index{filePathString.find_first_of('_')};

  if (index == std::string::npos) {
    return CL_UNEXPECTED(
      cl::Error::InvalidArgument,
      fmt::format("No underscore in \"{}\"!", filePathString));
  }

  constexpr std::size_t shortStringLength{8};

  try {
    std::string shortString{filePathString.substr(
      index + 1, /* skip the _ */
      shortStringLength)};

    constexpr std::size_t sensorOffset{10};

    if (isOld) {
      return LogLine{
        segmentationPointCountValue,
        filePathString,
        std::move(shortString),
        invalidSensor};
    }
    else {
      // preprocessed
      const std::string sensorString{
        filePathString.substr(index + sensorOffset)};

      std::uint64_t sensorValue{invalidSensor};

      if (sensorString == "Belly.csv") { sensorValue = 770; }
      else if (sensorString == "Chest.csv") {
        sensorValue = 772;
      }
      else if (sensorString == "LeftArm.csv") {
        sensorValue = 769;
      }
      else if (sensorString == "RightArm.csv") {
        sensorValue = 771;
      }
      else {
        return CL_UNEXPECTED(
          cl::Error::Parsing,
          fmt::format("Invalid sensorString: \"{}\"", sensorString));
      }

      return LogLine{
        segmentationPointCountValue,
        filePathString,
        std::move(shortString),
        sensorValue};
    }
  }
  catch (const std::out_of_range& ex) {
    return CL_UNEXPECTED(
      cl::Error::Parsing,
      fmt::format("Couldn't extract substring from \"{}\".", filePathString));
  }
}

std::uint64_t LogLine::segmentationPointCount() const noexcept
{
  return m_segmentationPointCount;
}

const cl::fs::Path& LogLine::filePath() const noexcept { return m_filePath; }

const std::string& LogLine::shortFileName() const noexcept
{
  return m_shortFileName;
}

std::uint64_t LogLine::sensor() const noexcept { return m_sensor; }

LogLine::LogLine(
  std::uint64_t  segmentationPointCount,
  cl::fs::Path&& filePath,
  std::string&&  shortFileName,
  std::uint64_t  sensor)
  : m_segmentationPointCount{segmentationPointCount}
  , m_filePath{std::move(filePath)}
  , m_shortFileName{std::move(shortFileName)}
  , m_sensor{sensor}
{
}
} // namespace cs
