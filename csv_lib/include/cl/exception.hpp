/*!
 * \file exception.hpp
 * \brief Exports the cl::Exception type and related utility macros.
 **/
#ifndef INCG_CL_EXCEPTION_HPP
#define INCG_CL_EXCEPTION_HPP
#include <cstddef>

#include <stdexcept>
#include <string>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/current_function.hpp>

namespace cl {
/*!
 * \brief The exception type for this code base.
 **/
class Exception : public std::runtime_error {
public:
  using base_type = std::runtime_error;

  /*!
   * \brief Constructs an `Exception`.
   * \param file The file in which the error occurred.
   * \param function The function in which the error occurred.
   * \param line The line in which the error occurred.
   * \param what_arg The error message.
   **/
  Exception(
    std::string        file,
    std::string        function,
    std::size_t        line,
    const std::string& what_arg);

  /*!
   * \brief Constructs an `Exception`.
   * \param file The file in which the error occurred.
   * \param function The function in which the error occurred.
   * \param line The line in which the error occurred.
   * \param what_arg The error message.
   **/
  Exception(
    std::string file,
    std::string function,
    std::size_t line,
    const char* what_arg);

  /*!
   * \brief Read accessor for the file.
   * \return The file where the error occurred.
   **/
  const std::string& file() const noexcept;

  /*!
   * \brief Read accesor for the function.
   * \return The function in which the error occurred.
   **/
  const std::string& function() const noexcept;

  /*!
   * \brief Read accesor for the line.
   * \return The line in which the error occurred.
   **/
  std::size_t line() const noexcept;

private:
  std::string m_file;     /*!< The file */
  std::string m_function; /*!< The function */
  std::size_t m_line;     /*!< The line */
};

/*!
 * \def CL_THROW
 * \brief Macro to throw a cl::Exception given an error message.
 **/
#define CL_THROW(what_arg) \
  throw ::cl::Exception { __FILE__, PL_CURRENT_FUNCTION, __LINE__, what_arg }

/*!
 * \def CL_THROW_FMT
 * \brief Convenience macro to throw a cl::Exception formatting the error
 *        message using fmt::format.
 **/
#define CL_THROW_FMT(fmt_str, ...) CL_THROW(::fmt::format(fmt_str, __VA_ARGS__))
} // namespace cl
#endif // INCG_CL_EXCEPTION_HPP
