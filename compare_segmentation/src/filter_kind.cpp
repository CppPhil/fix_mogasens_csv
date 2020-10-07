#include <ostream>

#include "filter_kind.hpp"

namespace cs {
std::ostream& operator<<(std::ostream& os, FilterKind filterKind)
{
  switch (filterKind) {
  case FilterKind::Butterworth: os << "Butterworth"; break;
  case FilterKind::MovingAverage: os << "MovingAverage"; break;
  }

  return os;
}
} // namespace cs
