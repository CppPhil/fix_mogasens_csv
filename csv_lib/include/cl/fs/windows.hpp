/*!
 * \file windows.hpp
 * \brief Contains Microsoft Windows specific functions.
 **/
#ifndef INCG_CL_FS_WINDOWS_HPP
#define INCG_CL_FS_WINDOWS_HPP
#include <string>

#include <pl/os.hpp>
#include <pl/string_view.hpp>

#if PL_OS == PL_OS_WINDOWS
#include <Windows.h>
#endif

namespace cl::fs {
#if PL_OS == PL_OS_WINDOWS
/*!
 * \brief Converts a UTF-8 encoded string to a UTF-16 encoded wstring.
 * \param utf8 The UTF-8 encoded string to convert.
 * \return The resulting UTF-16 string.
 * \note Most WINAPIs expect UTF-16 encoded wide character strings, but
 *       we don't want to pollute the code base with UTF-16 strings.
 * \warning Wide characters are only 16 bit wide on Microsoft Windows, they're
 *          32 bit on GNU / Linux.
 **/
[[nodiscard]] std::wstring utf8ToUtf16(pl::string_view utf8);

/*!
 * \brief Converts a UTF-16 encoded wide character string to UTF-8 string.
 * \param utf16 The UTF-16 encoded wide character string to convert.
 * \return The resulting UTF-8 string.
 * \note Most WINAPIs expect UTF-16 encoded wide character strings, but
 *       we don't want to pollute the code base with UTF-16 strings.
 * \warning Wide characters are only 16 bit wide on Microsoft Windows, they're
 *          32 bit on GNU / Linux.
 **/
[[nodiscard]] std::string utf16ToUtf8(pl::wstring_view utf16);

/*!
 * \brief Formats a WINAPI error code to a UTF-16 encoded wide character string.
 * \param errorCode The WINAPI error code.
 * \return The resulting UTF-16 encoded wide character string.
 * \note Most WINAPIs expect UTF-16 encoded wide character strings, but
 *       we don't want to pollute the code base with UTF-16 strings.
 * \warning Wide characters are only 16 bit wide on Microsoft Windows, they're
 *          32 bit on GNU / Linux.
 **/
[[nodiscard]] std::wstring formatError(DWORD errorCode);
#endif
} // cl::fs
#endif // INCG_CL_FS_WINDOWS_HPP
