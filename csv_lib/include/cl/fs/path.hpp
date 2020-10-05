#ifndef INCG_CL_FS_PATH_HPP
#define INCG_CL_FS_PATH_HPP
#include <iosfwd>
#include <string>

#include <pl/total_order.hpp>

namespace cl::fs {
class Path {
public:
  friend std::ostream& operator<<(std::ostream& os, const Path& path);

  friend bool operator<(const Path& lhs, const Path& rhs) noexcept;

  friend bool operator==(const Path& lhs, const Path& rhs) noexcept;

  explicit Path(std::string path);

  [[nodiscard]] bool exists() const noexcept;

  [[nodiscard]] bool isFile() const noexcept;

  [[nodiscard]] bool isDirectory() const noexcept;

  [[nodiscard]] const std::string& str() const noexcept;

private:
  std::string m_path;
};

PL_TOTAL_ORDER(Path)
} // namespace cl::fs
#endif // INCG_CL_FS_PATH_HPP
