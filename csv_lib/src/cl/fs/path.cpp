#include <cassert>
#include <cstring>

#include <ostream>
#include <utility>

#include <pl/os.hpp>

#if PL_OS == PL_OS_LINUX
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#elif PL_OS == PL_OS_WINDOWS
#include "cl/fs/windows.hpp"
#include <Windows.h>
#endif

#include "cl/fs/path.hpp"

namespace cl::fs {
std::ostream& operator<<(std::ostream& os, const Path& path)
{
  return os << path.m_path;
}

Path::Path(std::string path) : m_path{std::move(path)}
{
#if PL_OS == PL_OS_WINDOWS
  for (char& character : m_path) {
    if (character == '/') { character = '\\'; }
  }
#endif
}

bool Path::exists() const noexcept
{
#if PL_OS == PL_OS_LINUX
  struct stat statBuffer;
  std::memset(&statBuffer, 0, sizeof(statBuffer));
  const int statStatusCode{stat(m_path.c_str(), &statBuffer)};

  if (statStatusCode == -1) { return false; }

  return S_ISDIR(statBuffer.st_mode) || S_ISREG(statBuffer.st_mode);
#elif PL_OS == PL_OS_WINDOWS
  const std::wstring fileName{utf8ToUtf16(m_path)};

  const DWORD fileAttributes{GetFileAttributesW(fileName.c_str())};

  if (fileAttributes == INVALID_FILE_ATTRIBUTES) { return false; }

  if (fileAttributes & FILE_ATTRIBUTE_ARCHIVE) { return true; }

  if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) { return true; }

  if (fileAttributes & FILE_ATTRIBUTE_NORMAL) { return true; }

  if (fileAttributes & FILE_ATTRIBUTE_READONLY) { return true; }

  return false;
#endif
}

bool Path::isFile() const noexcept
{
#if PL_OS == PL_OS_LINUX
  struct stat statBuffer;
  std::memset(&statBuffer, 0, sizeof(statBuffer));
  const int statStatusCode{stat(m_path.c_str(), &statBuffer)};

  if (statStatusCode == -1) { return false; }

  return S_ISREG(statBuffer.st_mode);
#elif PL_OS == PL_OS_WINDOWS
  const std::wstring fileName{utf8ToUtf16(m_path)};

  const DWORD fileAttributes{GetFileAttributesW(fileName.c_str())};

  if (fileAttributes == INVALID_FILE_ATTRIBUTES) { return false; }

  if (fileAttributes & FILE_ATTRIBUTE_ARCHIVE) { return true; }

  if (fileAttributes & FILE_ATTRIBUTE_NORMAL) { return true; }

  if (fileAttributes & FILE_ATTRIBUTE_READONLY) { return true; }

  return false;
#endif
}

bool Path::isDirectory() const noexcept
{
#if PL_OS == PL_OS_LINUX
  struct stat statBuffer;
  std::memset(&statBuffer, 0, sizeof(statBuffer));
  const int statStatusCode{stat(m_path.c_str(), &statBuffer)};

  if (statStatusCode == -1) { return false; }

  return S_ISDIR(statBuffer.st_mode);
#elif PL_OS == PL_OS_WINDOWS
  const std::wstring fileName{utf8ToUtf16(m_path)};

  const DWORD fileAttributes{GetFileAttributesW(fileName.c_str())};

  if (fileAttributes == INVALID_FILE_ATTRIBUTES) { return false; }

  if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) { return true; }

  return false;
#endif
}

const std::string& Path::str() const noexcept { return m_path; }
} // namespace cl::fs
