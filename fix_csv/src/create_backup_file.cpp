#include "cl/fs/file.hpp"

#include "create_backup_file.hpp"

namespace fmc {
bool createBackupFile(
  const std::string& csvFilePath,
  const std::string& backupFilePath)
{
  const cl::fs::File csvFile{cl::fs::Path{csvFilePath}};
  return csvFile.copyTo(cl::fs::Path{backupFilePath});
}
} // namespace fmc
