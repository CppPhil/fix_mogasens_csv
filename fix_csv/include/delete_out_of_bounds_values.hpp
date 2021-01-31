#ifndef INCG_FMC_DELETE_OUT_OF_BOUNDS_VAULES_HPP
#define INCG_FMC_DELETE_OUT_OF_BOUNDS_VAULES_HPP
#include <string>
#include <vector>

#include "cl/error.hpp"

namespace fmc {
/*!
 * \brief Deletes out of bounds values from the raw CSV file read.
 * \param data Pointer to the matrix read from the raw CSV file.
 * \return An empty tl::expected on success or an error on error.
 **/
cl::Expected<void> deleteOutOfBoundsValues(
  std::vector<std::vector<std::string>>* data);
} // namespace fmc
#endif // INCG_FMC_DELETE_OUT_OF_BOUNDS_VAULES_HPP
