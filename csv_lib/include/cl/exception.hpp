#ifndef INCG_CL_EXCEPTION_HPP
#define INCG_CL_EXCEPTION_HPP
#include <cstddef>

#include <stdexcept>
#include <string>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/current_function.hpp>

namespace cl {
class Exception : public std::runtime_error {
public:
  using base_type = std::runtime_error;

  Exception(
    std::string        file,
    std::string        function,
    std::size_t        line,
    const std::string& what_arg);

  Exception(
    std::string file,
    std::string function,
    std::size_t line,
    const char* what_arg);

  const std::string& file() const noexcept;

  const std::string& function() const noexcept;

  std::size_t line() const noexcept;

private:
  std::string m_file;
  std::string m_function;
  std::size_t m_line;
};

#define CL_THROW(what_arg) \
  throw ::cl::Exception { __FILE__, PL_CURRENT_FUNCTION, __LINE__, what_arg }

#define CL_THROW_FMT(fmt_str, ...) CL_THROW(::fmt::format(fmt_str, __VA_ARGS__))
} // namespace cl
#endif // INCG_CL_EXCEPTION_HPP
