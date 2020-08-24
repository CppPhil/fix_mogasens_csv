#include <cassert>

#include <pl/os.hpp>
#include <pl/unreachable.hpp>

#if PL_OS == PL_OS_WINDOWS
#include "cl/fs/windows.hpp"
#include <wchar.h>
#endif

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "cl/fs/file_stream.hpp"

namespace cl::fs {
namespace {
const char* mapOpenMode(FileStream::OpenMode openMode)
{
  switch (openMode) {
  case FileStream::Read: return "rb";       /* read binary */
  case FileStream::Write: return "wb";      /* write binary */
  case FileStream::ReadWrite: return "wb+"; /* read / write binary */
  }

  PL_UNREACHABLE();
}
} // namespace

Expected<FileStream> FileStream::create(const File& file, OpenMode openMode)
{
  // The file size has to be determined here, because Microsoft Windows won't
  // allow you to get the file size while the file is opened.
  const std::int64_t fileByteCount{file.size()};

#if PL_OS == PL_OS_LINUX
  std::FILE* fileHandle{
    std::fopen(file.path().str().c_str(), mapOpenMode(openMode))};
#elif PL_OS == PL_OS_WINDOWS
  const std::wstring utf16Filename{utf8ToUtf16(file.path().str())};
  const std::wstring utf16Mode{utf8ToUtf16(mapOpenMode(openMode))};
  std::FILE* fileHandle{_wfopen(utf16Filename.c_str(), utf16Mode.c_str())};
#endif

  if (fileHandle == nullptr) {
    return CL_UNEXPECTED(
      Error::Filesystem, fmt::format("Couldn't open \"{}\"", file.path()));
  }

  constexpr auto errorSentinelValue{-1};

  if (fileByteCount == errorSentinelValue) {
    const int closeStatusCode{std::fclose(fileHandle)};
    (void)closeStatusCode;
    assert(closeStatusCode == 0 && "Couldn't close file handle.");
    return CL_UNEXPECTED(
      Error::Filesystem,
      fmt::format("Couldn't determine size of file \"{}\"", file.path()));
  }

  return FileStream{fileHandle, fileByteCount};
}

FileStream::FileStream(this_type&& other) noexcept
  : m_file{other.m_file}, m_size{other.m_size}
{
  other.m_file = nullptr;
  other.m_size = 0;
}

FileStream& FileStream::operator=(this_type&& other) noexcept
{
  std::swap(m_file, other.m_file);
  std::swap(m_size, other.m_size);
  return *this;
}

FileStream::~FileStream()
{
  if (m_file == nullptr) { return; }

  const int closeStatusCode{std::fclose(m_file)};

  (void)closeStatusCode;
  if (closeStatusCode != 0) {
    assert(false && "Failure to close file in ~FileStream");
  }
}

bool FileStream::write(const void* data, std::size_t byteCount)
{
  const std::size_t writeResult{std::fwrite(data, 1, byteCount, m_file)};

  return writeResult == byteCount;
}

std::vector<pl::byte> FileStream::readAll() const
{
  using namespace pl::integer_literals;
  std::vector<pl::byte> bytes(m_size, 0x00_byte);

  const std::size_t readResult{std::fread(bytes.data(), 1, m_size, m_file)};
  (void)readResult;
  assert(readResult == bytes.size() && "Read error in FileStream::readAll.");

  return bytes;
}

FileStream::FileStream(std::FILE* ptr, std::int64_t size) noexcept
  : m_file{ptr}, m_size{size}
{
}
} // namespace cl::fs
