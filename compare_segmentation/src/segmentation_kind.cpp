#include <ostream>

#include "segmentation_kind.hpp"

namespace cs {
std::ostream& operator<<(std::ostream& os, SegmentationKind segmentationKind)
{
  switch (segmentationKind) {
  case SegmentationKind::Both: os << "Both"; break;
  case SegmentationKind::Minima: os << "Minima"; break;
  case SegmentationKind::Maxima: os << "Maxima"; break;
  }

  return os;
}
} // namespace cs
