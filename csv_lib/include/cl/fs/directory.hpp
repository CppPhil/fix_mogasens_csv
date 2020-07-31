#ifndef INCG_CL_FS_DIRECTORY_HPP
#define INCG_CL_FS_DIRECTORY_HPP
#include <vector>

#include "fs/path.hpp"

namespace cl::fs {
class Directory {
public:
  explicit Directory(Path path);

  [[nodiscard]] bool exists() const noexcept;

  [[nodiscard]] std::vector<Path> list() const;

  const Path& path() const noexcept;

private:
  Path m_path;
};
} // namespace cl::fs
#endif // INCG_CL_FS_DIRECTORY_HPP
