#ifndef INCG_FMC_RESTORE_FROM_BACKUP_HPP
#define INCG_FMC_RESTORE_FROM_BACKUP_HPP
#include <string>

namespace fmc {
[[nodiscard]] bool restoreFromBackup(
  const std::string& csvFilePath,
  const std::string& backupFilePath);
} // namespace fmc
#endif // INCG_FMC_RESTORE_FROM_BACKUP_HPP
