#ifndef INCG_FE_EXCEPTION_HPP
#define INCG_FE_EXCEPTION_HPP
#include <cstddef>

#include <stdexcept>
#include <string>

namespace fe {
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
} // namespace fe
#endif // INCG_FE_EXCEPTION_HPP
