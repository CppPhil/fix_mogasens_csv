#ifndef INCG_CS_FILTER_KIND_HPP
#define INCG_CS_FILTER_KIND_HPP
#include <iosfwd>

namespace cs {
enum class FilterKind { Butterworth, MovingAverage };

std::ostream& operator<<(std::ostream& os, FilterKind filterKind);
} // namespace cs
#endif // INCG_CS_FILTER_KIND_HPP
