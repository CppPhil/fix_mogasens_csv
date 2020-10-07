#ifndef INCG_CS_SEGMENTATION_KIND_HPP
#define INCG_CS_SEGMENTATION_KIND_HPP
#include <iosfwd>

#include <pl/bitmask.hpp>
#include <pl/byte.hpp>

namespace cs {
enum class SegmentationKind : pl::byte {
  Minima = 0b0000'0001,
  Maxima = 0b0000'0010,
  Both   = Minima | Maxima
};

PL_ENABLE_BITMASK_OPERATORS(SegmentationKind)

std::ostream& operator<<(std::ostream& os, SegmentationKind segmentationKind);
} // namespace cs
#endif // INCG_CS_SEGMENTATION_KIND_HPP
