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
class FileStream {
public:
  enum OpenMode : std::uint8_t {
    Read      = 0b0000'0001,
    Write     = 0b0000'0010,
    ReadWrite = Read | Write
  };

  using this_type = FileStream;

  PL_NONCOPYABLE(FileStream);

  [[nodiscard]] static Expected<FileStream> create(
    const File& file,
    OpenMode    openMode);

  FileStream(this_type&& other) noexcept;

  this_type& operator=(this_type&& other) noexcept;

  ~FileStream();

  [[nodiscard]] bool write(const void* data, std::size_t byteCount);

  [[nodiscard]] std::vector<pl::byte> readAll() const;

private:
  FileStream(std::FILE* ptr, std::int64_t size) noexcept;

  std::FILE*   m_file;
  std::int64_t m_size;
};
} // namespace cl::fs
#endif // INCG_CL_FS_FILE_STREAM_HPP
