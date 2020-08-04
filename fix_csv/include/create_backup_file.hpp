#ifndef INCG_FMC_CREATE_BACKUP_FILE_HPP
#define INCG_FMC_CREATE_BACKUP_FILE_HPP
#include <string>

namespace fmc {
[[nodiscard]] bool createBackupFile(
  const std::string& csvFilePath,
  const std::string& backupFilePath);
} // namespace fmc
#endif // INCG_FMC_CREATE_BACKUP_FILE_HPP
