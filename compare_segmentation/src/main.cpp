#include <cstdio>
#include <cstdlib>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "log_files.hpp"
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

  // TODO: HERE: this will change BEGIN
  fmt::print("logs:\n");

  for (const cl::fs::Path& path : logs) { fmt::print("{}\n", path); }

  fmt::print("\n");
  fmt::print("old logs:\n");

  for (const cl::fs::Path& path : oldLogs) { fmt::print("{}\n", path); }
  // TODO: HERE: this will change END

  return EXIT_SUCCESS;
}
