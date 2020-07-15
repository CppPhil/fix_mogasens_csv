#ifndef INCG_CTG_RUN_ABOVE_THRESHOLD_HPP
#define INCG_CTG_RUN_ABOVE_THRESHOLD_HPP
#include <iosfwd>

#include "cl/data_set.hpp"

namespace ctg {
void runAboveThreshold(
  std::ostream&      aboveThresholdLogOutputFileStream,
  const cl::DataSet& dataSet);
} // namespace ctg
#endif // INCG_CTG_RUN_ABOVE_THRESHOLD_HPP
