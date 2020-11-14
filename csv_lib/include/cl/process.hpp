#ifndef INCG_CL_PROCESS_HPP
#define INCG_CL_PROCESS_HPP
#include <cstdio>

#include <pl/noncopyable.hpp>
#include <pl/string_view.hpp>

#include "error.hpp"

namespace cl {
class Process {
public:
  PL_NONCOPYABLE(Process);

  using this_type = Process;

  static Expected<Process> create(
    pl::string_view command,
    pl::string_view mode);

  Process(this_type&& other) noexcept;

  this_type& operator=(this_type&& other) noexcept;

  ~Process();

  std::FILE* file() noexcept;

  const std::FILE* file() const noexcept;

private:
  explicit Process(std::FILE* file) noexcept;

  std::FILE* m_file;
};
} // namespace cl
#endif // INCG_CL_PROCESS_HPP
