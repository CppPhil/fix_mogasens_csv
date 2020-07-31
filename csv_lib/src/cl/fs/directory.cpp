#include <iterator>
#include <utility>

#include <dlib/dir_nav.h>

#include <pl/algo/ranged_algorithms.hpp>

#include "cl/fs/directory.hpp"

namespace cl::fs {
Directory::Directory(Path path) : m_path{std::move(path)} {}

bool Directory::exists() const noexcept { return m_path.isDirectory(); }

std::vector<Path> Directory::list() const
{
  dlib::directory dir{m_path.str().c_str()};

  const std::vector<dlib::file> files{dir.get_files()};

  const std::vector<dlib::directory> dirs{dir.get_dirs()};

  std::vector<Path> result{};

  pl::algo::transform(
    files, std::back_inserter(result), [](const dlib::file& file) {
      return Path{file.full_name()};
    });
  pl::algo::transform(
    dirs, std::back_inserter(result), [](const dlib::directory& dir) {
      return Path{dir.full_name()};
    });

  return result;
}

const Path& Directory::path() const noexcept { return m_path; }
} // namespace cl::fs
