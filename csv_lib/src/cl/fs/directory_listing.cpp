#include <cassert>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/algo/erase.hpp>
#include <pl/algo/ranged_algorithms.hpp>
#include <pl/os.hpp>

#include <cl/fs/windows.hpp>

#if PL_OS == PL_OS_LINUX
#include <dirent.h>
#include <sys/types.h>
#elif PL_OS == PL_OS_WINDOWS
#include <Windows.h>
#else
#error "Unsupported operating system!"
#endif

#include <cl/fs/directory_listing.hpp>

namespace cl::fs {
Expected<std::vector<Path>> directoryListing(
  const Path&            directoryPath,
  DirectoryListingOption directoryListingOption)
{
  if (!directoryPath.isDirectory()) {
    return CL_UNEXPECTED(
      Error::Filesystem,
      fmt::format("\"{}\" is not a directory!", directoryPath));
  }

  const std::string& path{directoryPath.str()};
  std::vector<Path>  result{};

#if PL_OS == PL_OS_LINUX
  auto dir{opendir(path.c_str())};

  if (dir == nullptr) {
    return CL_UNEXPECTED(
      Error::Filesystem, fmt::format("opendir failed with \"{}\"!", path));
  }

  for (struct dirent* entry{nullptr}; (entry = readdir(dir)) != nullptr;) {
    result.emplace_back(std::string{entry->d_name});
  }

  if (closedir(dir) == EBADF) {
    return CL_UNEXPECTED(
      Error::Filesystem, fmt::format("Could not close \"{}\"!", path));
  }
#elif PL_OS == PL_OS_WINDOWS
  std::wstring utf16Path{utf8ToUtf16(path)};
  const DWORD  charactersNeeded{GetFullPathNameW(
    /* lpFileName */ utf16Path.c_str(),
    /* nBufferLength */ 0,
    /* lpBuffer */ nullptr,
    /* lpFilePart */ nullptr)};

  if (charactersNeeded == 0) {
    return CL_UNEXPECTED(
      Error::Filesystem,
      fmt::format(
        "\"{}\": GetFullPathNameW failed, line: {}!", path, __LINE__));
  }

  std::wstring buffer(charactersNeeded, L' ');

  const DWORD statusCode{GetFullPathNameW(
    /* lpFileName */ utf16Path.c_str(),
    /* nBufferLength */ buffer.size(),
    /* lpBuffer */ buffer.data(),
    /* lpFilePart */ nullptr)};

  if (statusCode == 0) {
    return CL_UNEXPECTED(
      Error::Filesystem,
      fmt::format(
        "\"{}\": GetFullPathNameW failed, line: {}!", path, __LINE__));
  }

  assert(
    (statusCode == (buffer.size() - 1U))
    && "Invalid return value of GetFullPathNameW");

  buffer.pop_back(); // Delete redundant trailing L'\0' character, the string is
                     // already null-terminated.

  // Appending \* makes Windows search IN the directory path given.
  const std::wstring   findFileInputWString{buffer + L"\\*"};
  const wchar_t* const findFileInput{findFileInputWString.c_str()};
  WIN32_FIND_DATAW     findData{};
  HANDLE               hFind{FindFirstFileW(findFileInput, &findData)};

  if (hFind == INVALID_HANDLE_VALUE) {
    return CL_UNEXPECTED(
      Error::Filesystem, fmt::format("\"{}\": FindFirstFileW failed.", path));
  }

  do {
    result.emplace_back(utf16ToUtf8(findData.cFileName));
  } while (FindNextFileW(hFind, &findData) != 0);

  if (GetLastError() != ERROR_NO_MORE_FILES) {
    FindClose(hFind);
    return CL_UNEXPECTED(
      Error::Filesystem,
      fmt::format(
        "\"{}\": GetLastError didn't return ERROR_NO_MORE_FILES!", path));
  }

  if (!FindClose(hFind)) {
    return CL_UNEXPECTED(
      Error::Filesystem, fmt::format("\"{}\": FindClose failed!", path));
  }
#endif

  if (directoryListingOption == DirectoryListingOption::ExcludeDotAndDotDot) {
    pl::algo::erase_if(result, [](const Path& currentPath) {
      return (currentPath.str() == ".") || (currentPath.str() == "..");
    });
  }

  pl::algo::sort(result);
  return result;
}
} // namespace cl::fs
