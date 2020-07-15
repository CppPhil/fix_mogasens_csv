#ifndef INCG_CTG_HALF_MAXIMUM_COMPARISON_VALUE_CALCULATOR_HPP
#define INCG_CTG_HALF_MAXIMUM_COMPARISON_VALUE_CALCULATOR_HPP
#include "cl/channel.hpp"
#include "cl/data_set.hpp"

namespace ctg {
long double halfMaximumComparisonValueCalculator(
  cl::Channel        channel,
  const cl::DataSet& dataSet);
} // namespace ctg
#endif // INCG_CTG_HALF_MAXIMUM_COMPARISON_VALUE_CALCULATOR_HPP
