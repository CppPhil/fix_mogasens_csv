#include <cstdio>
#include <cstdlib>

#include <fstream>
#include <string>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <csv.hpp>

#include <pl/unused.hpp>

#include "cl/fs/separator.hpp"

#include "csv_line.hpp"
#include "data_set_info.hpp"
#include "log_files.hpp"
#include "log_info.hpp"
#include "log_line.hpp"
#include "paths.hpp"

int main(int argc, char* argv[])
{
  PL_UNUSED(argc);
  fmt::print("{}: starting\n", argv[0]);

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

  constexpr char csvFilePath[]
    = "segmentation_comparison" CL_FS_SEPARATOR "out.csv";

  std::ofstream ofs{csvFilePath, std::ios_base::out | std::ios_base::trunc};

  if (!ofs) {
    fmt::print(stderr, "Could not open \"{}\" for writing!\n", csvFilePath);
    return EXIT_FAILURE;
  }

  auto csvWriter = csv::make_csv_writer(ofs);

  // Add column headers.
  csvWriter << std::vector<std::string>{
    "skip_window?",
    "delete too close?",
    "delete low variance?",
    "kind",
    "window size",
    "filter",
    "data set",
    "sensor",
    "push-ups",
    "segmentation points",
    "old / preprocessed"};

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

    if (!ifs) {
      fmt::print(
        stderr,
        "Could not open file \"{}\" for reading!\n",
        logInfo.logFilePath());
      return EXIT_FAILURE;
    }

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

      const cs::LogLine& logLine{*expectedLogLine};

      csvWriter << cs::CsvLineBuilder{}
                     .skipWindow(logInfo.skipWindow())
                     .deleteTooClose(logInfo.deleteTooClose())
                     .deleteLowVariance(logInfo.deleteLowVariance())
                     .kind(logInfo.segmentationKind())
                     .filter(logInfo.filterKind())
                     .windowSize(logInfo.windowSize())
                     .dataSet(logLine.shortFileName())
                     .sensor(logLine.sensor())
                     .pushUps(cs::pushUpCount(logLine.shortFileName()))
                     .segmentationPoints(logLine.segmentationPointCount())
                     .isOld(false)
                     .build();
    }

    // TODO: Check for the best setting.
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

    if (!ifs) {
      fmt::print(
        stderr,
        "Could not open file \"{}\" for reading!\n",
        logInfo.logFilePath());
      return EXIT_FAILURE;
    }

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

      const cs::LogLine& logLine{*expectedLogLine};

      csvWriter << cs::CsvLineBuilder{}
                     .skipWindow(logInfo.skipWindow())
                     .deleteTooClose(logInfo.deleteTooClose())
                     .deleteLowVariance(logInfo.deleteLowVariance())
                     .kind(logInfo.segmentationKind())
                     .filter(logInfo.filterKind())
                     .windowSize(logInfo.windowSize())
                     .dataSet(logLine.shortFileName())
                     .sensor(logInfo.sensor())
                     .pushUps(cs::pushUpCount(logLine.shortFileName()))
                     .segmentationPoints(logLine.segmentationPointCount())
                     .isOld(true)
                     .build();
    }

    // TODO: Check for the best setting.
  }

  fmt::print("{}: done\n", argv[0]);
  return EXIT_SUCCESS;
}
