#include <cstdio>
#include <cstdlib>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "log_files.hpp"
#include "log_info.hpp"
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

    // TODO: HERE
    [[maybe_unused]] const cs::LogInfo& logInfo{*expectedLogInfo};
  }

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

    // TODO: HERE
    [[maybe_unused]] const cs::LogInfo& logInfo{*expectedLogInfo};
  }

  return EXIT_SUCCESS;
}
