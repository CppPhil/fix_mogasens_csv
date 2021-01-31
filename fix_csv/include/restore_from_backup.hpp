#ifndef INCG_FMC_RESTORE_FROM_BACKUP_HPP
#define INCG_FMC_RESTORE_FROM_BACKUP_HPP
#include <string>

namespace fmc {
/*!
 * \brief Restore a file from a previously created backup file.
 * \param csvFilePath The path to the modified file that shall be restored.
 * \param backupFilePath The path to the backup of the original file.
 * \return true on success; otherwise false.
 **/
[[nodiscard]] bool restoreFromBackup(
  const std::string& csvFilePath,
  const std::string& backupFilePath);
} // namespace fmc
#endif // INCG_FMC_RESTORE_FROM_BACKUP_HPP
