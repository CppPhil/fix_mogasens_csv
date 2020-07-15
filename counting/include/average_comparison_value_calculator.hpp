#ifndef INCG_CTG_AVERAGE_COMPARISON_VALUE_CALCULATOR_HPP
#define INCG_CTG_AVERAGE_COMPARISON_VALUE_CALCULATOR_HPP
#include "cl/channel.hpp"
#include "cl/data_set.hpp"

namespace ctg {
long double averageComparisonValueCalculator(
  cl::Channel        channel,
  const cl::DataSet& dataSet);
} // namespace ctg
#endif // INCG_CTG_AVERAGE_COMPARISON_VALUE_CALCULATOR_HPP
