#include <cstdio>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "cl/fs/file.hpp"

#include "restore_from_backup.hpp"

namespace fmc {
bool restoreFromBackup(
  const std::string& csvFilePath,
  const std::string& backupFilePath)
{
  const cl::fs::Path modifiedFilePath{csvFilePath};
  cl::fs::File       csvFile{modifiedFilePath};
  if (!csvFile.remove()) {
    fmt::print(stderr, "Couldn't delete file \"{}\".\n", modifiedFilePath);
    return false;
  }
  cl::fs::File backupFile{cl::fs::Path{backupFilePath}};
  if (!backupFile.moveTo(modifiedFilePath)) {
    fmt::print(
      stderr,
      "Couldn't move file \"{}\" to \"{}\"!\n",
      backupFilePath,
      modifiedFilePath);
    return false;
  }
  return true;
}
} // namespace fmc
