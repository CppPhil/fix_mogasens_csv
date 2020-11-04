#ifndef INCG_CS_LOG_FILES_HPP
#define INCG_CS_LOG_FILES_HPP
#include <vector>

#include <pl/string_view.hpp>

#include <cl/error.hpp>
#include <cl/fs/path.hpp>

namespace cs {
/*!
 * \brief Fetches the paths to the log files in the given directory.
 * \param directoryPath The path to a directory to search for log files.
 * \return The log files found or an error.
 **/
[[nodiscard]] cl::Expected<std::vector<cl::fs::Path>> logFiles(
  pl::string_view directoryPath);
} // namespace cs
#endif // INCG_CS_LOG_FILES_HPP
