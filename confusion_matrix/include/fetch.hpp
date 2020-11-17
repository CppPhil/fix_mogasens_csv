#ifndef INCG_CM_FETCH_HPP
#define INCG_CM_FETCH_HPP
#include <cl/exception.hpp>

namespace cm {
/*!
 * \brief Fetches a value from a map for a given key.
 * \tparam Map The type of the map.
 * \tparam Key The type of the Key.
 * \param map The map to fetch from.
 * \param key The key to find the value for in `map`.
 * \return The value for `key` in `map`.
 * \throws cl::Exception if `key` is not found in `map`.
 **/
template<typename Map, typename Key>
auto fetch(const Map& map, const Key& key)
{
  const auto it{map.find(key)};

  if (it == map.end()) { CL_THROW_FMT("Could not find \"{}\" in map!", key); }

  // Return the value associated with the key.
  return it->second;
}
} // namespace cm
#endif // INCG_CM_FETCH_HPP
