/*!
 * \file error.hpp
 * \brief Exports the `Error` type and related utilities.
 **/
#ifndef INCG_CL_ERROR_HPP
#define INCG_CL_ERROR_HPP
#include <cstddef>

#include <iosfwd>
#include <string>

#include <tl/expected.hpp>

#include <pl/current_function.hpp>

#include "cl/to_string.hpp"

/*!
 * \def CL_ERROR_KIND
 * \brief X-Macro for the kinds of errors.
 **/
#define CL_ERROR_KIND              \
  CL_ERROR_KIND_X(Filesystem)      \
  CL_ERROR_KIND_X(InvalidArgument) \
  CL_ERROR_KIND_X(OutOfRange)      \
  CL_ERROR_KIND_X(Parsing)         \
  CL_ERROR_KIND_X(Logic)           \
  CL_ERROR_KIND_X(OperatingSystem)

namespace cl {
/*!
 * \brief Type used to represent different kinds of errors.
 **/
class [[nodiscard]] Error
{
public:
  /*!
   * \brief Unscoped enum type for the kinds of errors.
   **/
  enum Kind {
#define CL_ERROR_KIND_X(kind) kind,
    CL_ERROR_KIND
#undef CL_ERROR_KIND_X
  };

  /*!
   * \brief Prints `error` to `os`.
   * \param os The ostream to print to.
   * \param error The `Error` to print.
   * \return `os`.
   **/
  friend std::ostream& operator<<(std::ostream& os, const Error& error);

  /*!
   * \brief Constructs an `Error` object.
   * \param kind The enumerator corresponding to the kind of error.
   * \param file The file in which the error occurred.
   * \param function The function in which the error occurred.
   * \param line The line in which the error occurred.
   * \param message The error message of this `Error`.
   **/
  Error(
    Kind        kind,
    std::string file,
    std::string function,
    std::size_t line,
    std::string message);

  /*!
   * \brief Read accessor for the kind enumerator.
   * \return The kind of this `Error`.
   **/
  Kind kind() const noexcept;

  /*!
   * \brief Read accessor for the file string.
   * \return The file in which this `Error` occurred.
   **/
  const std::string& file() const noexcept;

  /*!
   * \brief Read accessor for the function string.
   * \return The function in which this `Error` occurred.
   **/
  const std::string& function() const noexcept;

  /*!
   * \brief Read accessor for the line number of this `Error`.
   * \return The line in which this `Error` occurred.
   **/
  std::size_t line() const noexcept;

  /*!
   * \brief Read accesor for the error message.
   * \return The error message.
   **/
  const std::string& message() const noexcept;

  /*!
   * \brief Throws this `Error` as a cl::Exception.
   **/
  [[noreturn]] void raise() const;

  /*!
   * \brief Converts this `Error` to a string.
   * \return A textual representation of this `Error`.
   **/
  [[nodiscard]] std::string to_string() const;

private:
  Kind        m_kind;     /*!< The kind enumerator */
  std::string m_file;     /*!< The file */
  std::string m_function; /*!< The function */
  std::size_t m_line;     /*!< The line */
  std::string m_message;  /*!< The error message */
};

/*!
 * \brief Type alias for tl::expected using cl::Error as the error type.
 * \tparam Ty The expected type.
 **/
template<typename Ty>
using Expected = tl::expected<Ty, Error>;
} // namespace cl

/*!
 * \def CL_UNEXPECTED
 * \brief Creates a cl::Expected object that indicates an error.
 **/
#define CL_UNEXPECTED(kind, message) \
  ::tl::make_unexpected(             \
    ::cl::Error{kind, __FILE__, PL_CURRENT_FUNCTION, __LINE__, message})
#endif // INCG_CL_ERROR_HPP
