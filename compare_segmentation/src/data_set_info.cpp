#include <cassert>

#include "data_set_info.hpp"

namespace cs {
std::uint64_t pushUpCount(pl::string_view dataSet)
{
  if (isFelix1(dataSet)) { return data_set_info<Felix1>::pushUps; }
  else if (isFelix2(dataSet)) {
    return data_set_info<Felix2>::pushUps;
  }
  else if (isFelix3(dataSet)) {
    return data_set_info<Felix3>::pushUps;
  }
  else if (isMarcelle1(dataSet)) {
    return data_set_info<Marcelle1>::pushUps;
  }
  else if (isMarcelle2(dataSet)) {
    return data_set_info<Marcelle2>::pushUps;
  }
  else if (isMarcelle3(dataSet)) {
    return data_set_info<Marcelle3>::pushUps;
  }
  else if (isMike1(dataSet)) {
    return data_set_info<Mike1>::pushUps;
  }
  else if (isMike2(dataSet)) {
    return data_set_info<Mike2>::pushUps;
  }
  else if (isMike3(dataSet)) {
    return data_set_info<Mike3>::pushUps;
  }
  else {
    assert(false && "invalid dataSet!");
    return 0;
  }
}
} // namespace cs
