#ifndef INCG_CL_PROCESS_HPP
#define INCG_CL_PROCESS_HPP
#include <cstdio>

#include <pl/noncopyable.hpp>
#include <pl/string_view.hpp>

#include "error.hpp"

namespace cl {
/*!
 * \brief Type representing an operating system process.
 *
 * An operating system process.
 * This type effectively wraps popen.
 * This is used to invoke the Python segmentor and read its stdout output
 * using C-style I/O through a std::FILE*.
 **/
class Process {
public:
  PL_NONCOPYABLE(Process); // Can't be copied.

  using this_type = Process;

  /*!
   * \brief Creates a process.
   * \param command The 'command' to invoke, e.g., the application or shell
   *                command to run.
   * \param mode The mode. Must be either "r" for reading or "w" for writing.
   * \return The process created.
   **/
  static Expected<Process> create(
    pl::string_view command,
    pl::string_view mode);

  /*!
   * \brief Move constructor.
   * \param other The other object to move construct from.
   * \warning `other` is left in a null state.
   **/
  Process(this_type&& other) noexcept;

  /*!
   * \brief Move assignment operator.
   * \param other The other object to move assign from.
   * \return *this.
   * \warning This process' std::FILE* handle will be handed over to `other`,
   *          leaving `other` to destroy it.
   **/
  this_type& operator=(this_type&& other) noexcept;

  /*!
   * \brief Destructor.
   **/
  ~Process();

  /*!
   * \brief Returns the underlying std::FILE*.
   **/
  std::FILE* file() noexcept;

  /*!
   * \brief Returns the underlying std::FILE*.
   **/
  const std::FILE* file() const noexcept;

private:
  explicit Process(std::FILE* file) noexcept;

  std::FILE* m_file;
};
} // namespace cl
#endif // INCG_CL_PROCESS_HPP
