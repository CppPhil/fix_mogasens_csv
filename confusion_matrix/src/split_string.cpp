#include <cstring>

#include <mutex>

#include "split_string.hpp"

namespace cm {
std::vector<std::string> splitString(
  std::string     string,
  pl::string_view splitBy)
{
  // Lock a mutex because strtox is stateful.
  static std::mutex                            mutex{};
  [[maybe_unused]] std::lock_guard<std::mutex> lockGuard{mutex};

  char* pch{std::strtok(string.data(), splitBy.c_str())};

  std::vector<std::string> result{};

  while (pch != nullptr) {
    result.push_back(pch);
    pch = std::strtok(nullptr, splitBy.c_str());
  }

  return result;
}
} // namespace cm
