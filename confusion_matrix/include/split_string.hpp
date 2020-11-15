#ifndef INCG_CM_SPLIT_STRING_HPP
#define INCG_CM_SPLIT_STRING_HPP
#include <string>
#include <vector>

#include <pl/string_view.hpp>

namespace cm {
/*!
 * \brief Splits `string` by `splitBy`.
 * \param string The string to split.
 * \param splitBy What to split `string` by.
 * \return The resulting strings.
 **/
[[nodiscard]] std::vector<std::string> splitString(
  std::string     string,
  pl::string_view splitBy);
} // namespace cm
#endif // INCG_CM_SPLIT_STRING_HPP
