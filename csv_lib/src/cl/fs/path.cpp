#include <cstring>

#include <ostream>
#include <utility>

#include <pl/os.hpp>

#if PL_OS == PL_OS_LINUX
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#elif == PL_OS == PL_OS_WINDOWS
#include "cl/fs/windows.hpp"
#include <Windows.h>
#endif

#include "cl/fs/path.hpp"

namespace cl::fs {
std::ostream& operator<<(std::ostream& os, const Path& path)
{
  return os << path.m_path;
}

Path::Path(std::string path) : m_path{std::move(path)} {}

bool Path::exists() const noexcept
{
#if PL_OS == PL_OS_LINUX
  struct stat buf;
  std::memset(&buf, 0, sizeof(buf));
  const int i{stat(m_path.c_str(), &buf)};

  if (i == -1) { return false; }

  return S_ISDIR(buf.st_mode) || S_ISREG(buf.st_mode);
#elif PL_OS == PL_OS_WINDOWS
  const std::wstring fileName{utf8ToUtf16(m_path)};

  const DWORD dw{GetFileAttributesW(fileName.c_str())};

  if (dw & FILE_ATTRIBUTE_ARCHIVE) { return true; }

  if (dw & FILE_ATTRIBUTE_DIRECTORY) { return true; }

  if (dw & FILE_ATTRIBUTE_NORMAL) { return true; }

  if (dw & FILE_ATTRIBUTE_READONLY) { return true; }

  return false;
#endif
}

bool Path::isFile() const noexcept
{
#if PL_OS == PL_OS_LINUX
  struct stat buf;
  std::memset(&buf, 0, sizeof(buf));
  const int i{stat(m_path.c_str(), &buf)};

  if (i == -1) { return false; }

  return S_ISREG(buf.st_mode);
#elif PL_OS == PL_OS_WINDOWS
  const std::wstring fileName{utf8ToUtf16(m_path)};

  const DWORD dw{GetFileAttributesW(fileName.c_str())};

  if (dw & FILE_ATTRIBUTE_ARCHIVE) { return true; }

  if (dw & FILE_ATTRIBUTE_NORMAL) { return true; }

  if (dw & FILE_ATTRIBUTE_READONLY) { return true; }

  return false;
#endif
}

bool Path::isDirectory() const noexcept
{
#if PL_OS == PL_OS_LINUX
  struct stat buf;
  std::memset(&buf, 0, sizeof(buf));
  const int i{stat(m_path.c_str(), &buf)};

  if (i == -1) { return false; }

  return S_ISDIR(buf.st_mode);
#elif PL_OS == PL_OS_WINDOWS
  const std::wstring fileName{utf8ToUtf16(m_path)};

  const DWORD dw{GetFileAttributesW(fileName.c_str())};

  if (dw & FILE_ATTRIBUTE_DIRECTORY) { return true; }

  return false;
#endif
}

const std::string& Path::str() const noexcept { return m_path; }
} // namespace cl::fs
