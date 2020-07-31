#include <cassert>
#include <cstddef>

#include "cl/fs/windows.hpp"

#if PL_OS == PL_OS_WINDOWS
#include <Windows.h>
#endif

namespace cl::fs {
#if PL_OS == PL_OS_WINDOWS
namespace {
[[nodiscard]] std::size_t utf8ToUtf16RequiredChars(pl::string_view utf8)
{
  return MultiByteToWideChar(
    /* CodePage */ CP_UTF8,
    /* dwFlags */ 0,
    /* lpMultiByteStr */ utf8.c_str(),
    /* cbMultiByte */ -1,
    /* lpWideCharStr */ nullptr,
    /* cchWideChar */ 0);
}

[[nodiscard]] std::size_t utf16ToUtf8RequiredBytes(pl::wstring_view utf16)
{
  return WideCharToMultiByte(
    /* CodePage */ CP_UTF8,
    /* dwFlags */ 0,
    /* lpWideCharStr */ utf16.c_str(),
    /* cchWideChar */ -1,
    /* lpMultiByteStr */ nullptr,
    /* cbMultiByte */ 0,
    /* lpDefaultChar */ nullptr,
    /* lpUsedDefaultChar */ nullptr);
}
} // namespace

std::wstring utf8ToUtf16(pl::string_view utf8)
{
  static_assert(sizeof(char) == 1, "char isn't 1 byte wide");

  const std::size_t charsNeeded{utf8ToUtf16RequiredChars(utf8)};
  std::wstring      buf(charsNeeded, L' ');

  const int i{MultiByteToWideChar(
    /* CodePage */ CP_UTF8,
    /* dwFlags */ 0,
    /* lpMultiByteStr */ utf8.c_str(),
    /* cbMultiByte */ -1,
    /* lpWideCharStr */ buf.data(),
    /* cchWideChar */ buf.size())};

  if (i == 0) {
    assert(false && "Couldn't convert utf8 to utf16");
    return L"";
  }

  buf.pop_back();
  return buf;
}

std::string utf16ToUtf8(pl::wstring_view utf16)
{
  static_assert(sizeof(wchar_t) == 2, "wchar_t isn't 2 bytes wide");

  const std::size_t bytesNeeded{utf16ToUtf8RequiredBytes(utf16)};
  std::string       buf(bytesNeeded, ' ');

  const int i{WideCharToMultiByte(
    /* CodePage */ CP_UTF8,
    /* dwFlags */ 0,
    /* lpWideCharStr */ utf16.c_str(),
    /* cchWideChar */ -1,
    /* lpMultiByteStr */ buf.data(),
    /* cbMultiByte */ buf.size(),
    /* lpDefaultChar */ nullptr,
    /* lpUsedDefaultChar */ nullptr)};

  if (i == 0) {
    assert(false && "Couldn't convert utf16 to utf8");
    return "";
  }

  buf.pop_back();
  return buf;
}
#endif
} // cl::fs
