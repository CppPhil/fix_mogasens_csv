#ifndef INCG_CL_FS_FILE_HPP
#define INCG_CL_FS_FILE_HPP
#include <cstdint>

#include "cl/fs/path.hpp"

namespace cl::fs {
/*!
 * \brief Represents a file.
 **/
class File {
public:
  /*!
   * \brief Creates a File from the given `path`.
   * \param path The path to use.
   **/
  explicit File(Path path);

  /*!
   * \brief Determines if this file exists.
   * \return true if the file exists; otherwise false.
   **/
  [[nodiscard]] bool exists() const noexcept;

  /*!
   * \brief Creates this file.
   * \return true if the file was successfully created; otherwise false.
   * \note Will fail if the file already exists.
   **/
  [[nodiscard]] bool create() const noexcept;

  /*!
   * \brief Copies this file in the filesystem.
   * \param copyToPath The path to copy to.
   * \return true if the file was successfully copied to `copyToPath`; otherwise
   *         false.
   * \warning There should be no file that already exists at `copyToPath`.
   **/
  [[nodiscard]] bool copyTo(const Path& copyToPath) const noexcept;

  /*!
   * \brief Moves this file in the filesystem.
   * \param newPath The path to move this file to.
   * \return true if the file was successfully moved to `newPath`; otherwise
   *         false.
   **/
  [[nodiscard]] bool moveTo(const Path& newPath);

  /*!
   * \brief Deletes this file.
   * \return true if deleting succeeded; otherwise false.
   **/
  [[nodiscard]] bool remove() noexcept;

  /*!
   * \brief Determines the size of this file in bytes.
   * \return The size of this file in bytes or -1 on error.
   * \warning Returns -1 on error.
   **/
  [[nodiscard]] std::int64_t size() const noexcept;

  /*!
   * \brief Read accessor for the path of this file.
   * \return The path of this file.
   **/
  const Path& path() const noexcept;

private:
  Path m_path;
};
} // namespace cl::fs
#endif // INCG_CL_FS_FILE_HPP
