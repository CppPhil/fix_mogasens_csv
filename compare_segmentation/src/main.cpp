#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <fstream>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <csv.hpp>

#include <pl/algo/ranged_algorithms.hpp>

#include "cl/fs/separator.hpp"
#include "cl/to_string.hpp"

#include "csv_line.hpp"
#include "data_set_info.hpp"
#include "log_files.hpp"
#include "log_info.hpp"
#include "log_line.hpp"
#include "mode.hpp"
#include "paths.hpp"

int main(int argc, char* argv[])
{
  constexpr int thisApplicationIndex{0};
  constexpr int modeIndex{1};
  constexpr int expectedArgc{2};

  const pl::string_view thisApplication{argv[thisApplicationIndex]};

  fmt::print(stderr, "{}: starting\n", thisApplication);

  if (argc != expectedArgc) {
    fmt::print(
      stderr,
      "argc was {}, but it was expected to be {}! Exiting.",
      argc,
      expectedArgc);
    return EXIT_FAILURE;
  }

  const cl::Expected<cs::Mode> expectedMode{cs::parseMode(argv[modeIndex])};

  if (!expectedMode.has_value()) {
    fmt::print(stderr, "Could not parse mode out of \"{}\"!", argv[modeIndex]);
    return EXIT_FAILURE;
  }

  const cs::Mode mode{*expectedMode};
  fmt::print(stderr, "{}: Mode is \"{}\".\n", thisApplication, mode);

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

  // The CSV file to write to.
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
    "repetitions",
    "segmentation points",
    "old / preprocessed"};

  // This shall be replaced with a proper value.
  cs::LogInfo  bestOne{};
  std::int64_t bestTotalDistance{};

  struct LogInfoDistancePair {
    cs::LogInfo  logInfo;
    std::int64_t distanceScore;
  };

  std::vector<LogInfoDistancePair> logInfoDistancePairVector{};

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
    std::int64_t       currentLogInfoTotalDistance{};

    std::ifstream ifs{logInfo.logFilePath().str()};

    if (!ifs) {
      fmt::print(
        stderr,
        "Could not open file \"{}\" for reading!\n",
        logInfo.logFilePath());
      return EXIT_FAILURE;
    }

    std::vector<cs::LogLine> logLines{};

    for (std::string line{}; std::getline(ifs, line);) {
      const cl::Expected<cs::LogLine> expectedLogLine{cs::LogLine::parse(line)};

      if (!expectedLogLine.has_value()) {
        fmt::print(
          stderr,
          "Could not parse line \"{}\" from file \"{}\", line: {}, error: {}\n",
          line,
          logInfo.logFilePath(),
          __LINE__,
          expectedLogLine.error());
        return EXIT_FAILURE;
      }

      const cs::LogLine& logLine{*expectedLogLine};

      switch (mode) {
      case cs::Mode::AllDataSets:
        // No filter, keep all LogLines.
        logLines.push_back(logLine);
        break;
      case cs::Mode::AllPushUps:
        // All push-ups: Do not add Squats.
        if (!pl::strcontains(logLine.filePath().str(), "Squats")) {
          logLines.push_back(logLine);
        }
        break;
      case cs::Mode::PushUps200Hz:
        // Only add those that contain "liegestuetzen".
        // The new (200Hz) ones all contain "liegestuetzen" as part of their
        // file name.
        if (pl::strcontains(logLine.filePath().str(), "liegestuetzen")) {
          logLines.push_back(logLine);
        }
        break;
      case cs::Mode::PushUps250Hz:
        // Only add those that are "old" push-up data sets.
        // -> Add anything that does not contain "liegestuetzen" and
        //    also does not contain "Squats", as to keep only the "old"
        //    push-up data sets (sampled at 250Hz).
        if (
          !pl::strcontains(logLine.filePath().str(), "liegestuetzen")
          && !pl::strcontains(logLine.filePath().str(), "Squats")) {
          logLines.push_back(logLine);
        }
        break;
      case cs::Mode::Squats:
        // Only keep the Squats:
        // -> Only add LogLines that contain "Squats".
        if (pl::strcontains(logLine.filePath().str(), "Squats")) {
          logLines.push_back(logLine);
        }
        break;
      }
    }

    pl::algo::stable_sort(
      logLines, [](const cs::LogLine& lhs, const cs::LogLine& rhs) {
        return lhs.fileName().value() < rhs.fileName().value();
      });

    for (const cs::LogLine& logLine : logLines) {
      const std::int64_t expectedRepetitionCount{static_cast<std::int64_t>(
        cs::repetitionCount(logLine.filePath().str()))};

      const std::int64_t distance{std::abs(
        static_cast<std::int64_t>(logLine.segmentationPointCount())
        - expectedRepetitionCount)};

      currentLogInfoTotalDistance += distance;

      csvWriter << cs::CsvLineBuilder{}
                     .skipWindow(logInfo.skipWindow())
                     .deleteTooClose(logInfo.deleteTooClose())
                     .deleteLowVariance(logInfo.deleteLowVariance())
                     .kind(logInfo.segmentationKind())
                     .filter(logInfo.filterKind())
                     .windowSize(logInfo.windowSize())
                     .dataSet(
                       logLine.fileName().value_or(logLine.filePath().str()))
                     .sensor(logLine.sensor())
                     .repetitions(
                       static_cast<std::uint64_t>(expectedRepetitionCount))
                     .segmentationPoints(logLine.segmentationPointCount())
                     .isOld(false)
                     .build();
    }

    fmt::print("{}  distance: {}\n", logInfo, currentLogInfoTotalDistance);

    csvWriter << std::vector<std::string>{
      "score (lower is better): ", cl::to_string(currentLogInfoTotalDistance)};
    csvWriter << std::vector<std::string>{""}; // Write empty line.

    logInfoDistancePairVector.push_back(
      LogInfoDistancePair{logInfo, currentLogInfoTotalDistance});

    if (
      !bestOne.isInitialized()
      || (currentLogInfoTotalDistance < bestTotalDistance)) {
      bestOne           = logInfo;
      bestTotalDistance = currentLogInfoTotalDistance;
    }
  }

  fmt::print(
    "\nBest configuration is\n  {}\n  with total distance {}\n",
    bestOne,
    bestTotalDistance);

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
          "Could not parse line \"{}\" from file \"{}\", line: {}, error: {}\n",
          line,
          logInfo.logFilePath(),
          __LINE__,
          expectedLogLine.error());
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
                     .dataSet(
                       logLine.fileName().value_or(logLine.filePath().str()))
                     .sensor(logInfo.sensor())
                     .repetitions(cs::repetitionCount(logLine.filePath().str()))
                     .segmentationPoints(logLine.segmentationPointCount())
                     .isOld(true)
                     .build();
    }

    csvWriter << std::vector<std::string>{""}; // Write empty line.
  }

  pl::algo::stable_sort(
    logInfoDistancePairVector,
    [](const LogInfoDistancePair& lhs, const LogInfoDistancePair& rhs) {
      return lhs.distanceScore < rhs.distanceScore;
    });

  fmt::print("\nSorted by distance score (best first):\n");

  for (const LogInfoDistancePair& currentLogInfoDistancePair :
       logInfoDistancePairVector) {
    fmt::print(
      "{}, distance: {}\n",
      currentLogInfoDistancePair.logInfo,
      currentLogInfoDistancePair.distanceScore);
  }

  fmt::print(stderr, "{}: done\n", argv[0]);
  return EXIT_SUCCESS;
}
