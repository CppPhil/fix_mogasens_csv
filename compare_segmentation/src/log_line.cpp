#include <utility>

#include <pl/strcontains.hpp>

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
  [[maybe_unused]] const bool isOld{isOldPath(line)};

  [[maybe_unused]] static constexpr char regularExpressionString[]
    = R"~(^.*\.py:\s(\d+)\ssegmentation\spoints\sfound\sin\s"(resources/[[:graph:]]+\.csv)"\.$)~";

  // TODO: HERE

  return CL_UNEXPECTED(cl::Error::InvalidArgument, "NOT YET IMPLEMENTED");
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
