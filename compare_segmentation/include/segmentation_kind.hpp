#ifndef INCG_CS_SEGMENTATION_KIND_HPP
#define INCG_CS_SEGMENTATION_KIND_HPP
#include <iosfwd>

#include <pl/bitmask.hpp>
#include <pl/byte.hpp>

namespace cs {
/*!
 * \brief The segmentation kind (bitflag type)
 **/
enum class SegmentationKind : pl::byte {
  Minima = 0b0000'0001,    /*!< Segmentation by local minima */
  Maxima = 0b0000'0010,    /*!< Segmentation by local maxima */
  Both   = Minima | Maxima /*!< Segmentation by both local extrema */
};

// Generate the bitwise operators.
PL_ENABLE_BITMASK_OPERATORS(SegmentationKind)

/*!
 * \brief Prints a SegmentationKind to an ostream.
 * \param os The ostream to print to.
 * \param segmentationKind The SegmentationKind to print.
 * \return os
 **/
std::ostream& operator<<(std::ostream& os, SegmentationKind segmentationKind);
} // namespace cs
#endif // INCG_CS_SEGMENTATION_KIND_HPP
