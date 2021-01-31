#ifndef INCG_FMC_DELETE_NON_BOSCH_SENSORS_HPP
#define INCG_FMC_DELETE_NON_BOSCH_SENSORS_HPP
#include <string>
#include <vector>

namespace fmc {
/*!
 * \brief Routine to delete anything that isn't a Bosch sensor.
 * \param data Pointer to the matrix read from the raw CSV file.
 **/
void deleteNonBoschSensors(std::vector<std::vector<std::string>>* data);
} // namespace fmc
#endif // INCG_FMC_DELETE_NON_BOSCH_SENSORS_HPP
