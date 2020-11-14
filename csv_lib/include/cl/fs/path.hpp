#ifndef INCG_CL_FS_PATH_HPP
#define INCG_CL_FS_PATH_HPP
#include <functional>
#include <iosfwd>
#include <string>

#include <pl/annotations.hpp>
#include <pl/total_order.hpp>

namespace cl::fs {
/*!
 * \brief A filesystem path.
 **/
class Path {
public:
  /*!
   * \brief Prints a Path to an ostream.
   * \param os the ostream to print to.
   * \param path The path to print.
   * \return `os`
   **/
  friend std::ostream& operator<<(std::ostream& os, const Path& path);

  /*!
   * \brief Checks if `lhs` is less than `rhs`.
   * \param lhs The left hand side operand.
   * \param rhs The right hand side operand.
   * \return true if lhs < rhs; otherwise false.
   **/
  friend bool operator<(const Path& lhs, const Path& rhs) noexcept;

  /*!
   * \brief Equality compares `lhs` and `rhs`.
   * \param lhs The left hand side operand.
   * \param rhs The right hand side operand.
   * \return true if `lhs` and `rhs` are equal.
   **/
  friend bool operator==(const Path& lhs, const Path& rhs) noexcept;

  /*!
   * \brief Creates a path.
   * \param path The string to construct from.
   **/
  PL_IMPLICIT Path(std::string path);

  /*!
   * \brief Creates a path.
   * \param path The string to construct from.
   **/
  PL_IMPLICIT Path(const char* path);

  /*!
   * \brief Checks if the path exists.
   * \return true if the path exists; otherwise false.
   **/
  [[nodiscard]] bool exists() const noexcept;

  /*!
   * \brief Checks if the path is a file.
   * \return true if the path is a file; otherwise false.
   **/
  [[nodiscard]] bool isFile() const noexcept;

  /*!
   * \brief Checks if the path is a directory.
   * \return true if the path is a directory; otherwise false.
   **/
  [[nodiscard]] bool isDirectory() const noexcept;

  /*!
   * \brief Read accessor for the underlying string.
   * \return The underlying string.
   **/
  [[nodiscard]] const std::string& str() const noexcept;

private:
  std::string m_path;
};

PL_TOTAL_ORDER(Path)
} // namespace cl::fs

namespace std {
template<>
struct hash<::cl::fs::Path> {
  size_t operator()(const ::cl::fs::Path& path) const
  {
    return hash<string>{}(path.str());
  }
};
} // namespace std
#endif // INCG_CL_FS_PATH_HPP
