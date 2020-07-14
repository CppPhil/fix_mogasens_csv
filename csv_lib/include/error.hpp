#ifndef INCG_FE_ERROR_HPP
#define INCG_FE_ERROR_HPP
#include <cstddef>

#include <iosfwd>
#include <string>

#include <tl/expected.hpp>

#include <pl/current_function.hpp>

#define FE_ERROR_KIND         \
  FE_ERROR_KIND_X(Filesystem) \
  FE_ERROR_KIND_X(InvalidArgument)

namespace fe {
class [[nodiscard]] Error
{
public:
  enum Kind {
#define FE_ERROR_KIND_X(kind) kind,
    FE_ERROR_KIND
#undef FE_ERROR_KIND_X
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

private:
  Kind        m_kind;
  std::string m_file;
  std::string m_function;
  std::size_t m_line;
  std::string m_message;
};

template<typename Ty>
using Expected = tl::expected<Ty, Error>;
} // namespace fe

#define FE_UNEXPECTED(kind, message) \
  ::tl::make_unexpected(             \
    ::fe::Error{kind, __FILE__, PL_CURRENT_FUNCTION, __LINE__, message})
#endif // INCG_FE_ERROR_HPP
