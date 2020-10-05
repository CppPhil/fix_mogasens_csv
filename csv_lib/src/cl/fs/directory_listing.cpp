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
  const char* const  szPath{path.c_str()};

  std::vector<Path> result{};

#if PL_OS == PL_OS_LINUX
  auto dir{opendir(szPath)};

  if (dir == nullptr) {
    return CL_UNEXPECTED(
      Error::Filesystem, fmt::format("opendir failed with \"{}\"!", szPath));
  }

  for (struct dirent* entry{nullptr}; (entry = readdir(dir)) != nullptr;) {
    result.emplace_back(std::string{entry->d_name});
  }

  if (closedir(dir) == EBADF) {
    return CL_UNEXPECTED(
      Error::Filesystem, fmt::format("Could not close \"{}\"!", szPath));
  }
#elif PL_OS == PL_OS_WINDOWS
  std::wstring utf16Path{utf8ToUtf16(path)};
  pl::algo::replace(utf16Path, L'/', L'\\');

  WCHAR buffer[MAX_PATH]{};

  const DWORD statusCode{GetFullPathNameW(
    /* lpFileName */ utf16Path.c_str(),
    /* nBufferLength */ MAX_PATH,
    /* lpBuffer */ buffer,
    /* lpFilePart */ nullptr)};

  if (statusCode == 0) {
    return CL_UNEXPECTED(
      Error::Filesystem, fmt::format("\"{}\": GetFullPathNameW failed!", path));
  }

  const wchar_t* const wszPath{buffer};
  WIN32_FIND_DATAW     findData{};
  HANDLE               hFind{FindFirstFileW(wszPath, &findData)};

  if (hFind == INVALID_HANDLE_VALUE) {
    return CL_UNEXPECTED(
      Error::Filesystem, fmt::format("\"{}\": FindFirstFileW failed.", path));
  }

  do {
    if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
      result.emplace_back(utf16ToUtf8(findData.cFileName));
    }
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
