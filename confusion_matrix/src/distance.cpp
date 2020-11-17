#include <algorithm>

#include "distance.hpp"

namespace cm {
std::uint64_t distance(std::uint64_t a, std::uint64_t b)
{
  const auto [smaller, larger] = std::minmax(a, b);
  return larger - smaller;
};
} // namespace cm
