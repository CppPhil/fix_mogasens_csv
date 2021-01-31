#ifndef INCG_CTG_PERCENTAGE_OF_HPP
#define INCG_CTG_PERCENTAGE_OF_HPP
#include <cstddef>

namespace ctg {
/*!
 * \brief Calculates what percentage `amount` is of `totalCount`.
 * \param amount The first argument.
 * \param totalCount The second argument.
 * \return The percentage.
 **/
constexpr long double percentageOf(
  std::size_t amount,
  std::size_t totalCount) noexcept
{
  return static_cast<long double>(amount) / static_cast<long double>(totalCount)
         * 100.0L;
}
} // namespace ctg
#endif // INCG_CTG_PERCENTAGE_OF_HPP
