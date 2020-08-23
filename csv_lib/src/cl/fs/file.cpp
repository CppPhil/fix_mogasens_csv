#include <cassert>
#include <cstdio>

#include <string>
#include <utility>

#include <pl/os.hpp>

#if PL_OS == PL_OS_WINDOWS
#include "cl/fs/windows.hpp"
#include <Windows.h>
#elif PL_OS == PL_OS_LINUX
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include "cl/fs/file.hpp"

namespace cl::fs {
File::File(Path path) : m_path{std::move(path)} {}

bool File::exists() const noexcept { return m_path.isFile(); }

bool File::create() const noexcept
{
  if (exists()) { return false; }

#if PL_OS == PL_OS_LINUX
  std::FILE* fileHandle{std::fopen(m_path.str().c_str(), "wb")};

  if (fileHandle == nullptr) { return false; }

  const int statusCode{std::fclose(fileHandle)};
  (void)statusCode;
  assert(statusCode == 0 && "Couldn't close file handle in file::create");

  return true;
#elif PL_OS == PL_OS_WINDOWS
  const std::wstring utf16Path{utf8ToUtf16(m_path.str())};
  HANDLE             fileHandle{CreateFileW(
    /* lpFileName */ utf16Path.c_str(),
    /* dwDesiredAccess */ GENERIC_READ | GENERIC_WRITE,
    /* dwShareMode */ 0,
    /* lpSecurityAttributes */ nullptr,
    /* dwCreationDisposition */ CREATE_NEW,
    /* dwFlagsAndAttributes */ FILE_ATTRIBUTE_NORMAL,
    /* hTemplateFile */ nullptr)};

  if (fileHandle == INVALID_HANDLE_VALUE) { return false; }

  return CloseHandle(fileHandle) != 0;
#endif
}

bool File::copyTo(const Path& copyToPath) const noexcept
{
#if PL_OS == PL_OS_LINUX
  std::FILE* source{std::fopen(m_path.str().c_str(), "rb")};

  if (source == nullptr) { return false; }

  std::FILE* destination{std::fopen(copyToPath.str().c_str(), "wb")};

  if (destination == nullptr) {
    const int destinationCloseStatusCode{std::fclose(source)};
    (void)destinationCloseStatusCode;
    assert(
      destinationCloseStatusCode == 0
      && "failure to close file handle in file::copyTo");
    return false;
  }

  for (int character{0}; (character = std::getc(source)) != EOF;) {
    const int writeByteStatusCode{std::putc(character, destination)};
    (void)writeByteStatusCode;
    assert(writeByteStatusCode != EOF && "Failure to write!");
  }

  int closeStatusCode{std::fclose(source)};
  assert(closeStatusCode == 0 && "Couldn't close file handle.");
  closeStatusCode = std::fclose(destination);
  assert(closeStatusCode == 0 && "Couldn't close destination file handle.");
  (void)closeStatusCode;

  return true;
#elif PL_OS == PL_OS_WINDOWS
  const std::wstring utf16Path{utf8ToUtf16(m_path.str())};
  const std::wstring newFileName{utf8ToUtf16(copyToPath.str())};

  const BOOL copyFileStatusCode{CopyFileW(
    /* lpExistingFileName */ utf16Path.c_str(),
    /* lpNewFileName */ newFileName.c_str(),
    /* bFailIfExists */ FALSE)};

  return copyFileStatusCode != 0;
#endif
}

bool File::moveTo(const Path& newPath)
{
#if PL_OS == PL_OS_LINUX
  return std::rename(m_path.str().c_str(), newPath.str().c_str()) == 0;
#elif PL_OS == PL_OS_WINDOWS
  const std::wstring thisPath{utf8ToUtf16(m_path.str())};
  const std::wstring newPathWstring{utf8ToUtf16(newPath.str())};

  return MoveFileExW(
           /* lpExistingFileName */ thisPath.c_str(),
           /* lpNewFileName */ newPathWstring.c_str(),
           /* dwFlags */ MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING
             | MOVEFILE_WRITE_THROUGH)
         != 0;
#endif
}

bool File::remove() noexcept
{
#if PL_OS == PL_OS_LINUX
  return std::remove(m_path.str().c_str()) == 0;
#elif PL_OS == PL_OS_WINDOWS
  const std::wstring utf16Path{utf8ToUtf16(m_path.str())};
  const BOOL         deleteFileStatusCode{DeleteFileW(utf16Path.c_str())};
  return deleteFileStatusCode != 0;
#endif
}

std::int64_t File::size() const noexcept
{
  constexpr std::int64_t errorResultValue{-1};

#if PL_OS == PL_OS_LINUX
  struct stat buf {
  };
  const int statusCode{stat(m_path.str().c_str(), &buf)};

  if (statusCode == -1) { return errorResultValue; }

  return buf.st_size;
#elif PL_OS == PL_OS_WINDOWS
  const std::wstring utf16Path{utf8ToUtf16(m_path.str())};
  const HANDLE       fileHandle{CreateFileW(
    /* lpFileName */ utf16Path.c_str(),
    /* dwDesiredAccess */ GENERIC_READ,
    /* dwShareMode */ 0,
    /* lpSecurityAttributes */ nullptr,
    /* dwCreationDisposition */ OPEN_EXISTING,
    /* dwFlagsAndAttributes */ FILE_ATTRIBUTE_NORMAL,
    /* hTemplateFile */ nullptr)};

  if (fileHandle == INVALID_HANDLE_VALUE) {
      return errorResultValue; 
  }

  LARGE_INTEGER fileSize{};
  const BOOL    statusCode{GetFileSizeEx(
    /* hFile */ fileHandle,
    /* lpFileSize */ &fileSize)};

  if (statusCode == 0) {
    CloseHandle(/* hObject */ fileHandle);
    return errorResultValue;
  }

  if (CloseHandle(/* hObject */ fileHandle) == 0) { return errorResultValue; }

  return fileSize.QuadPart;
#endif
}

const Path& File::path() const noexcept { return m_path; }
} // namespace cl::fs
