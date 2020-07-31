#include <utility>

#include "cl/exception.hpp"

namespace cl {
Exception::Exception(
  std::string        file,
  std::string        function,
  std::size_t        line,
  const std::string& what_arg)
  : base_type{what_arg}
  , m_file{std::move(file)}
  , m_function{std::move(function)}
  , m_line{line}
{
}

Exception::Exception(
  std::string file,
  std::string function,
  std::size_t line,
  const char* what_arg)
  : base_type{what_arg}
  , m_file{std::move(file)}
  , m_function{std::move(function)}
  , m_line{line}
{
}

const std::string& Exception::file() const noexcept { return m_file; }

const std::string& Exception::function() const noexcept { return m_function; }

std::size_t Exception::line() const noexcept { return m_line; }
} // namespace cl
