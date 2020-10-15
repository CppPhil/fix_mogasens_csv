#include <cassert>
#include <cstdio>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "data_set_info.hpp"

namespace cs {
std::uint64_t repetitionCount(pl::string_view dataSet)
{
  if (containsFelix1(dataSet)) { return data_set_info<Felix1>::repetitions; }
  else if (containsFelix2(dataSet)) {
    return data_set_info<Felix2>::repetitions;
  }
  else if (containsFelix3(dataSet)) {
    return data_set_info<Felix3>::repetitions;
  }
  else if (containsMarcelle1(dataSet)) {
    return data_set_info<Marcelle1>::repetitions;
  }
  else if (containsMarcelle2(dataSet)) {
    return data_set_info<Marcelle2>::repetitions;
  }
  else if (containsMarcelle3(dataSet)) {
    return data_set_info<Marcelle3>::repetitions;
  }
  else if (containsMike1(dataSet)) {
    return data_set_info<Mike1>::repetitions;
  }
  else if (containsMike2(dataSet)) {
    return data_set_info<Mike2>::repetitions;
  }
  else if (containsMike3(dataSet)) {
    return data_set_info<Mike3>::repetitions;
  }
  else if (containsJan1(dataSet)) {
    return data_set_info<Jan1>::repetitions;
  }
  else if (containsJan2(dataSet)) {
    return data_set_info<Jan2>::repetitions;
  }
  else if (containsJan3(dataSet)) {
    return data_set_info<Jan3>::repetitions;
  }
  else if (containsAndre1(dataSet)) {
    return data_set_info<Andre1>::repetitions;
  }
  else if (containsAndre2(dataSet)) {
    return data_set_info<Andre2>::repetitions;
  }
  else if (containsAndre3(dataSet)) {
    return data_set_info<Andre3>::repetitions;
  }
  else if (containsAndreSquats1(dataSet) && !containsAndreSquats2(dataSet)) {
    return data_set_info<AndreSquats1>::repetitions;
  }
  else if (containsAndreSquats2(dataSet)) {
    return data_set_info<AndreSquats2>::repetitions;
  }
  else if (containsLucas1(dataSet)) {
    return data_set_info<Lucas1>::repetitions;
  }
  else if (containsLucas2(dataSet)) {
    return data_set_info<Lucas2>::repetitions;
  }
  else if (containsLucas3(dataSet)) {
    return data_set_info<Lucas3>::repetitions;
  }
  else {
    fmt::print(stderr, "repetitionCount: invalid data set \"{}\".\n", dataSet);
    assert(false && "invalid dataSet!");
    PL_THROW_WITH_SOURCE_INFO(
      NoSuchDataSetException,
      fmt::format("\"{}\" is not a known data set!", dataSet));
  }
}
} // namespace cs
