#ifndef INCG_FMC_CREATE_BACKUP_FILE_HPP
#define INCG_FMC_CREATE_BACKUP_FILE_HPP
#include <string>

namespace fmc {
/*!
 * \brief Creates a backup of a file.
 * \param csvFilePath The path to the file to create a backup of.
 * \param backupFilePath Where to copy the file to.
 * \return true on success; false otherwise.
 **/
[[nodiscard]] bool createBackupFile(
  const std::string& csvFilePath,
  const std::string& backupFilePath);
} // namespace fmc
#endif // INCG_FMC_CREATE_BACKUP_FILE_HPP
