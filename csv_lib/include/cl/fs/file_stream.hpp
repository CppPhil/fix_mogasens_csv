#ifndef INCG_CL_FS_FILE_STREAM_HPP
#define INCG_CL_FS_FILE_STREAM_HPP
#include <cstddef>
#include <cstdint>
#include <cstdio>

#include <vector>

#include <pl/byte.hpp>
#include <pl/noncopyable.hpp>

#include "cl/error.hpp"
#include "cl/fs/file.hpp"

namespace cl::fs {
/*!
 * \brief A binary file stream.
 **/
class FileStream {
public:
  /*!
   * \brief The file open mode.
   **/
  enum OpenMode : std::uint8_t {
    Read      = 0b0000'0001, /*!< Read only access */
    Write     = 0b0000'0010, /*!< Write only access */
    ReadWrite = Read | Write /*!< Read and write access */
  };

  using this_type = FileStream;

  PL_NONCOPYABLE(FileStream);

  /*!
   * \brief Creates a file stream.
   * \param file The file to open.
   * \param openMode The open mode to use.
   * \return The file stream or an error.
   **/
  [[nodiscard]] static Expected<FileStream> create(
    const File& file,
    OpenMode    openMode);

  /*!
   * \brief Move constructs from `other`.
   * \param other The file stream to move construct from.
   **/
  FileStream(this_type&& other) noexcept;

  /*!
   * \brief Move assigns `other` to this file stream.
   * \param other The file stream to move assign to this file stream.
   * \return *this
   **/
  this_type& operator=(this_type&& other) noexcept;

  /*!
   * \brief Closes this file stream.
   **/
  ~FileStream();

  /*!
   * \brief Writes data to the file.
   * \param data Pointer to the beginning of the memory region to write.
   * \param byteCount The amount of bytes to write, starting from `data`.
   * \return true on success; otherwise false.
   **/
  [[nodiscard]] bool write(const void* data, std::size_t byteCount);

  /*!
   * \brief Reads the entire file into RAM.
   * \return The bytes read.
   **/
  [[nodiscard]] std::vector<pl::byte> readAll() const;

private:
  FileStream(std::FILE* ptr, std::int64_t size) noexcept;

  std::FILE*   m_file;
  std::int64_t m_size;
};
} // namespace cl::fs
#endif // INCG_CL_FS_FILE_STREAM_HPP
