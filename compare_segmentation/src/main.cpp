#include <cstdio>
#include <cstdlib>

#include <fstream>
#include <string>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "log_files.hpp"
#include "log_info.hpp"
#include "log_line.hpp"
#include "paths.hpp"

int main()
{
  const cl::Expected<std::vector<cl::fs::Path>> expectedLogs{
    cs::logFiles(cs::logPath)};

  if (!expectedLogs.has_value()) {
    fmt::print(
      stderr,
      "Couldn't fetch log files from \"{}\": {}\n",
      cs::logPath,
      expectedLogs.error());
    return EXIT_FAILURE;
  }

  const std::vector<cl::fs::Path>&              logs{*expectedLogs};
  const cl::Expected<std::vector<cl::fs::Path>> expectedOldLogs{
    cs::logFiles(cs::oldLogPath)};

  if (!expectedOldLogs.has_value()) {
    fmt::print(
      stderr,
      "Couldn't fetch old log files from \"{}\": {}\n",
      cs::oldLogPath,
      expectedOldLogs.error());
    return EXIT_FAILURE;
  }

  const std::vector<cl::fs::Path>& oldLogs{*expectedOldLogs};

  // PREPROCESSED
  for (const cl::fs::Path& preprocessedPath : logs) {
    const cl::Expected<cs::LogInfo> expectedLogInfo{
      cs::LogInfo::create(preprocessedPath)};

    if (!expectedLogInfo.has_value()) {
      fmt::print(
        stderr,
        "Couldn't fetch LogInfo for preprocessedPath: {}\n",
        expectedLogInfo.error());
      return EXIT_FAILURE;
    }

    const cs::LogInfo& logInfo{*expectedLogInfo};

    std::ifstream ifs{logInfo.logFilePath().str()};

    for (std::string line{}; std::getline(ifs, line);) {
      const cl::Expected<cs::LogLine> expectedLogLine{cs::LogLine::parse(line)};

      if (!expectedLogLine.has_value()) {
        fmt::print(
          stderr,
          "Could not parse line \"{}\" from file \"{}\", line: {}\n",
          line,
          logInfo.logFilePath(),
          __LINE__);
        return EXIT_FAILURE;
      }

      [[maybe_unused]] const cs::LogLine& logLine{*expectedLogLine};

      // TODO: Write to CSV.
    }
  }

  // OLD
  for (const cl::fs::Path& oldPath : oldLogs) {
    const cl::Expected<cs::LogInfo> expectedLogInfo{
      cs::LogInfo::create(oldPath)};

    if (!expectedLogInfo.has_value()) {
      fmt::print(
        stderr,
        "Couldn't fetch LogInfo for oldPath: {}\n",
        expectedLogInfo.error());
      return EXIT_FAILURE;
    }

    const cs::LogInfo& logInfo{*expectedLogInfo};

    std::ifstream ifs{logInfo.logFilePath().str()};

    for (std::string line{}; std::getline(ifs, line);) {
      const cl::Expected<cs::LogLine> expectedLogLine{cs::LogLine::parse(line)};

      if (!expectedLogLine.has_value()) {
        fmt::print(
          stderr,
          "Could not parse line \"{}\" from file \"{}\", line: {}\n",
          line,
          logInfo.logFilePath(),
          __LINE__);
        return EXIT_FAILURE;
      }

      [[maybe_unused]] const cs::LogLine& logLine{*expectedLogLine};

      // TODO: Write to CSV.
    }
  }

  return EXIT_SUCCESS;
}
