#ifndef INCG_CL_DOS2UNIX_HPP
#define INCG_CL_DOS2UNIX_HPP
#include <cstddef>

#include <vector>

#include <pl/byte.hpp>

namespace cl {
/*!
 * \brief Converts DOS / Microsoft Windows line endings to UNIX line endings.
 * \param p The beginning of the data to convert.
 * \param size The size of the data to convert in bytes.
 * \return The resulting byte array.
 **/
[[nodiscard]] std::vector<pl::byte> dos2unix(const void* p, std::size_t size);
} // namespace cl
#endif // INCG_CL_DOS2UNIX_HPP