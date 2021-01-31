#ifndef INCG_FMC_REMOVE_ZEROS_FROM_FIELD_HPP
#define INCG_FMC_REMOVE_ZEROS_FROM_FIELD_HPP
#include <string>

namespace fmc {
/*!
 * \brief Deletes extraneous zeros from a cell value.
 * \param field Pointer to a string of a cell from a CSV file.
 **/
void removeZerosFromField(std::string* field);
} // namespace fmc
#endif // INCG_FMC_REMOVE_ZEROS_FROM_FIELD_HPP
