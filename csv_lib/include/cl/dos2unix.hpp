#ifndef INCG_CL_DOS2UNIX_HPP
#define INCG_CL_DOS2UNIX_HPP
#include <cstddef>

#include <vector>

#include <pl/byte.hpp>

namespace cl {
[[nodiscard]] std::vector<pl::byte> dos2unix(const void* p, std::size_t size);
} // namespace cl
#endif // INCG_CL_DOS2UNIX_HPP