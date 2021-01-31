#ifndef INCG_CTG_RUN_ABOVE_THRESHOLD_HPP
#define INCG_CTG_RUN_ABOVE_THRESHOLD_HPP
#include <iosfwd>

#include "cl/data_set.hpp"

namespace ctg {
/*!
 * \brief Routine to find the aboe threshold values.
 * \param aboveThresholdLogOutputFileStream The stream to write to.
 * \param dataSet The data set.
 **/
void runAboveThreshold(
  std::ostream&      aboveThresholdLogOutputFileStream,
  const cl::DataSet& dataSet);
} // namespace ctg
#endif // INCG_CTG_RUN_ABOVE_THRESHOLD_HPP
