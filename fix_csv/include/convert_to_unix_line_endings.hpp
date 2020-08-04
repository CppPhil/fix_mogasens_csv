#ifndef INCG_FMC_CONVERT_TO_UNIX_LINE_ENDINGS_HPP
#define INCG_FMC_CONVERT_TO_UNIX_LINE_ENDINGS_HPP
#include <string>

namespace fmc {
[[nodiscard]] bool convertToUnixLineEndings(const std::string& csvPath);
} // namespace fmc
#endif // INCG_FMC_CONVERT_TO_UNIX_LINE_ENDINGS_HPP
