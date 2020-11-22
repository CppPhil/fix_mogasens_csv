#ifndef INCG_CM_DISTANCE_HPP
#define INCG_CM_DISTANCE_HPP
#include <cstdint>

namespace cm {
/*!
 * \brief Calculates the distance between `a` and `b`.
 * \param a The first parameter.
 * \param b The second parameter.
 * \return The difference between `a` and `b`.
 **/
[[nodiscard]] std::uint64_t distance(std::uint64_t a, std::uint64_t b);
} // namespace cm
#endif // INCG_CM_DISTANCE_HPP
