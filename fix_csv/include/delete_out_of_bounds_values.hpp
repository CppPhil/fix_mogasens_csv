#ifndef INCG_FMC_DELETE_OUT_OF_BOUNDS_VAULES_HPP
#define INCG_FMC_DELETE_OUT_OF_BOUNDS_VAULES_HPP
#include <string>
#include <vector>

#include "cl/error.hpp"

namespace fmc {
cl::Expected<void> deleteOutOfBoundsValues(
  std::vector<std::vector<std::string>>* data);
} // namespace fmc
#endif // INCG_FMC_DELETE_OUT_OF_BOUNDS_VAULES_HPP
