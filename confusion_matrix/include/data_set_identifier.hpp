#ifndef INCG_CM_DATA_SET_IDENTIFIER_HPP
#define INCG_CM_DATA_SET_IDENTIFIER_HPP
#include <iosfwd>

#include <cl/fs/path.hpp>

namespace cm {
#define CM_DATA_SET_IDENTIFIER             \
  CM_DATA_SET_IDENTIFIER_X(Felix_11_17_39) \
  CM_DATA_SET_IDENTIFIER_X(Felix_12_50_00) \
  CM_DATA_SET_IDENTIFIER_X(Felix_13_00_09) \
  CM_DATA_SET_IDENTIFIER_X(Mike_14_07_33)  \
  CM_DATA_SET_IDENTIFIER_X(Mike_14_14_32)  \
  CM_DATA_SET_IDENTIFIER_X(Mike_14_20_28)  \
  CM_DATA_SET_IDENTIFIER_X(Marsi_14_59_59) \
  CM_DATA_SET_IDENTIFIER_X(Marsi_15_13_22) \
  CM_DATA_SET_IDENTIFIER_X(Marsi_15_31_36) \
  CM_DATA_SET_IDENTIFIER_X(Jan_1)          \
  CM_DATA_SET_IDENTIFIER_X(Jan_2)          \
  CM_DATA_SET_IDENTIFIER_X(Jan_3)          \
  CM_DATA_SET_IDENTIFIER_X(Andre_1)        \
  CM_DATA_SET_IDENTIFIER_X(Andre_2)        \
  CM_DATA_SET_IDENTIFIER_X(Andre_3)        \
  CM_DATA_SET_IDENTIFIER_X(Andre_Squats_1) \
  CM_DATA_SET_IDENTIFIER_X(Andre_Squats_2) \
  CM_DATA_SET_IDENTIFIER_X(Lucas_1)        \
  CM_DATA_SET_IDENTIFIER_X(Lucas_2)        \
  CM_DATA_SET_IDENTIFIER_X(Lucas_3)

enum class DataSetIdentifier {
#define CM_DATA_SET_IDENTIFIER_X(enm) enm,
  CM_DATA_SET_IDENTIFIER
#undef CM_DATA_SET_IDENTIFIER_X
};

/*!
 * \brief Prints a DataSetIdentifier to an ostream.
 * \param os The ostream to print to.
 * \param dsi The `DataSetIdentifier` to print.
 * \return `os`
 **/
std::ostream& operator<<(std::ostream& os, DataSetIdentifier dsi);

/*!
 * \brief Converts a path to a CSV file to the corresponding DataSetIdentifier.
 * \param path The path.
 * \return The resulting DataSetIdentifier.
 * \throws cl::Exception if `path` is unrecognized.
 **/
DataSetIdentifier toDataSetIdentifier(const cl::fs::Path& path);
} // namespace cm
#endif // INCG_CM_DATA_SET_IDENTIFIER_HPP
