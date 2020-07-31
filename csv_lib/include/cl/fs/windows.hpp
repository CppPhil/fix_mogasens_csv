#ifndef INCG_CL_FS_WINDOWS_HPP
#define INCG_CL_FS_WINDOWS_HPP
#include <string>

#include <pl/os.hpp>
#include <pl/string_view.hpp>

namespace cl::fs {
#if PL_OS == PL_OS_WINDOWS
std::wstring utf8ToUtf16(pl::string_view utf8);

std::string utf16ToUtf8(pl::wstring_view utf16);
#endif
} // cl::fs
#endif // INCG_CL_FS_WINDOWS_HPP
