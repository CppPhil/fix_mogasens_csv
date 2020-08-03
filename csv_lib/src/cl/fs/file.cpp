#include <cassert>
#include <cstdio>

#include <string>
#include <utility>

#include <dlib/dir_nav.h>

#include <pl/os.hpp>

#if PL_OS == PL_OS_WINDOWS
#include "cl/fs/windows.hpp"
#include <Windows.h>
#endif

#include "cl/fs/file.hpp"

namespace cl::fs {
File::File(Path path) : m_path{std::move(path)} {}

bool File::exists() const noexcept { return m_path.isFile(); }

bool File::create() const noexcept
{
  if (exists()) { return false; }

  std::FILE* fh{std::fopen(m_path.str().c_str(), "wb")};

  if (fh == nullptr) { return false; }

  const int errC{std::fclose(fh)};
  (void)errC;
  assert(errC == 0 && "Couldn't close file handle in file::create");

  return true;
}

bool File::copyTo(const Path& copyToPath) const noexcept
{
#if PL_OS == PL_OS_LINUX
  std::FILE* fh{std::fopen(m_path.str().c_str(), "rb")};

  if (fh == nullptr) { return false; }

  std::FILE* dest{std::fopen(copyToPath.str().c_str(), "wb")};

  if (dest == nullptr) {
    const int i{std::fclose(fh)};
    (void)i;
    assert(i == 0 && "failure to close file handle in file::copyTo");
    return false;
  }

  for (int ch; (ch = std::getc(fh)) != EOF;) {
    const int errC{std::putc(ch, dest)};
    (void)errC;
    assert(errC != EOF && "Failure to write!");
  }

  int errorCode{std::fclose(fh)};
  assert(errorCode == 0 && "Couldn't close file handle.");
  errorCode = std::fclose(dest);
  assert(errorCode == 0 && "Couldn't close destination file handle.");
  (void)errorCode;

  return true;
#elif PL_OS == PL_OS_WINDOWS
  const std::wstring wstr{utf8ToUtf16(m_path.str())};
  const std::wstring newFileName{utf8ToUtf16(copyToPath.str())};

  const BOOL errC{CopyFileW(
    /* lpExistingFileName */ wstr.c_str(),
    /* lpNewFileName */ newFileName.c_str(),
    /* bFailIfExists */ TRUE)};

  return errC != 0;
#endif
}

bool File::moveTo(const Path& newPath)
{
  return std::rename(m_path.str().c_str(), newPath.str().c_str()) == 0;
}

bool File::remove() noexcept { return std::remove(m_path.str().c_str()) == 0; }

std::int64_t File::size() const noexcept
{
  try {
    return dlib::file{m_path.str().c_str()}.size();
  }
  catch ([[maybe_unused]] const dlib::file::file_not_found& ex) {
    return -1;
  }
}

const Path& File::path() const noexcept { return m_path; }
} // namespace cl::fs
