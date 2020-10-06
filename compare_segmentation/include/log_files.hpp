#ifndef INCG_CS_LOG_FILES_HPP
#define INCG_CS_LOG_FILES_HPP
#include <vector>

#include <pl/string_view.hpp>

#include <cl/error.hpp>
#include <cl/fs/path.hpp>

namespace cs {
[[nodiscard]] cl::Expected<std::vector<cl::fs::Path>> logFiles(
  pl::string_view directoryPath);
} // namespace cs
#endif // INCG_CS_LOG_FILES_HPP
