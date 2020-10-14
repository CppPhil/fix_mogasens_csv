#include <cassert>
#include <cstdio>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "data_set_info.hpp"

namespace cs {
std::uint64_t repetitionCount(pl::string_view dataSet)
{
  if (isFelix1(dataSet)) { return data_set_info<Felix1>::repetitions; }
  else if (isFelix2(dataSet)) {
    return data_set_info<Felix2>::repetitions;
  }
  else if (isFelix3(dataSet)) {
    return data_set_info<Felix3>::repetitions;
  }
  else if (isMarcelle1(dataSet)) {
    return data_set_info<Marcelle1>::repetitions;
  }
  else if (isMarcelle2(dataSet)) {
    return data_set_info<Marcelle2>::repetitions;
  }
  else if (isMarcelle3(dataSet)) {
    return data_set_info<Marcelle3>::repetitions;
  }
  else if (isMike1(dataSet)) {
    return data_set_info<Mike1>::repetitions;
  }
  else if (isMike2(dataSet)) {
    return data_set_info<Mike2>::repetitions;
  }
  else if (isMike3(dataSet)) {
    return data_set_info<Mike3>::repetitions;
  }
  else if (isJan1(dataSet)) {
    return data_set_info<Jan1>::repetitions;
  }
  else if (isJan2(dataSet)) {
    return data_set_info<Jan2>::repetitions;
  }
  else if (isJan3(dataSet)) {
    return data_set_info<Jan3>::repetitions;
  }
  else if (isAndre1(dataSet)) {
    return data_set_info<Andre1>::repetitions;
  }
  else if (isAndre2(dataSet)) {
    return data_set_info<Andre2>::repetitions;
  }
  else if (isAndre3(dataSet)) {
    return data_set_info<Andre3>::repetitions;
  }
  else if (isAndreSquats1(dataSet) && !isAndreSquats2(dataSet)) {
    return data_set_info<AndreSquats1>::repetitions;
  }
  else if (isAndreSquats2(dataSet)) {
    return data_set_info<AndreSquats2>::repetitions;
  }
  else if (isLucas1(dataSet)) {
    return data_set_info<Lucas1>::repetitions;
  }
  else if (isLucas2(dataSet)) {
    return data_set_info<Lucas2>::repetitions;
  }
  else if (isLucas3(dataSet)) {
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
