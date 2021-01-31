#ifndef INCG_CL_TO_STRING_HPP
#define INCG_CL_TO_STRING_HPP
#include <ios>
#include <locale>
#include <sstream>
#include <string>
#include <type_traits>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/meta/remove_cvref.hpp>

namespace cl {
/*!
 * \brief Converts something to a string (if possible).
 * \tparam Ty The type of the object to convert to a string.
 * \param ty The object to convert to a string.
 * \return The resulting string.
 **/
template<typename Ty>
inline std::string to_string(const Ty& ty)
{
  using Type = pl::meta::remove_cvref_t<Ty>;

  // Use fmtlib's formatter for integers.
  if constexpr (
    std::is_same_v<
      Type,
      int> || std::is_same_v<Type, long> || std::is_same_v<Type, long long> || std::is_same_v<Type, unsigned> || std::is_same_v<Type, unsigned long> || std::is_same_v<Type, unsigned long long>) {
    fmt::format_int formatter{ty};
    return formatter.str();
  }
  else {
    // For anything else: try to print it to a stream.
    std::ostringstream oss{};
    oss.imbue(std::locale::classic()); // Use classic locale, just in case
                                       // someone changed the global one.
    oss << std::boolalpha; // Print booleans as "true" or "false" rather than
                           // "1" or "0".
    oss << ty;             // Print the actual object to `oss`.
    return oss.str();      // Convert to a string.
  }
}
} // namespace cl
#endif // INCG_CL_TO_STRING_HPP
