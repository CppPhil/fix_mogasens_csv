#ifndef INCG_CS_FILTER_KIND_HPP
#define INCG_CS_FILTER_KIND_HPP
#include <iosfwd>

namespace cs {
/*!
 * \brief Type for the different kinds of filters.
 **/
enum class FilterKind { Butterworth, MovingAverage };

/*!
 * \brief Prints a FilterKind to an ostream.
 * \param os The ostream to print to.
 * \param filterKind The FilterKind to print.
 * \return `os`
 **/
std::ostream& operator<<(std::ostream& os, FilterKind filterKind);
} // namespace cs
#endif // INCG_CS_FILTER_KIND_HPP
