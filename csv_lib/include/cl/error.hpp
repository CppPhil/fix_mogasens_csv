#ifndef INCG_CL_ERROR_HPP
#define INCG_CL_ERROR_HPP
#include <cstddef>

#include <iosfwd>
#include <string>

#include <tl/expected.hpp>

#include <pl/current_function.hpp>

#include "cl/to_string.hpp"

#define CL_ERROR_KIND              \
  CL_ERROR_KIND_X(Filesystem)      \
  CL_ERROR_KIND_X(InvalidArgument) \
  CL_ERROR_KIND_X(OutOfRange)

namespace cl {
class [[nodiscard]] Error
{
public:
  enum Kind {
#define CL_ERROR_KIND_X(kind) kind,
    CL_ERROR_KIND
#undef CL_ERROR_KIND_X
  };

  friend std::ostream& operator<<(std::ostream& os, const Error& error);

  Error(
    Kind        kind,
    std::string file,
    std::string function,
    std::size_t line,
    std::string message);

  Kind kind() const noexcept;

  const std::string& file() const noexcept;

  const std::string& function() const noexcept;

  std::size_t line() const noexcept;

  const std::string& message() const noexcept;

  [[noreturn]] void raise() const;

  [[nodiscard]] std::string to_string() const;

private:
  Kind        m_kind;
  std::string m_file;
  std::string m_function;
  std::size_t m_line;
  std::string m_message;
};

template<typename Ty>
using Expected = tl::expected<Ty, Error>;
} // namespace cl

#define CL_UNEXPECTED(kind, message) \
  ::tl::make_unexpected(             \
    ::cl::Error{kind, __FILE__, PL_CURRENT_FUNCTION, __LINE__, message})
#endif // INCG_CL_ERROR_HPP
