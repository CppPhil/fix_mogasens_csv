#ifndef INCG_CL_TO_STRING_HPP
#define INCG_CL_TO_STRING_HPP
#include <sstream>
#include <string>
#include <type_traits>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/meta/remove_cvref.hpp>

namespace cl {
template<typename Ty>
inline auto to_string(const Ty& ty)
{
  using Type = pl::meta::remove_cvref_t<Ty>;

  if constexpr (
    std::is_same_v<
      Type,
      int> || std::is_same_v<Type, long> || std::is_same_v<Type, long long> || std::is_same_v<Type, unsigned> || std::is_same_v<Type, unsigned long> || std::is_same_v<Type, unsigned long long>) {
    fmt::format_int formatter{ty};
    return formatter.str();
  }
  else {
    std::ostringstream oss{};
    oss << ty;
    return oss.str();
  }
}
} // namespace cl
#endif // INCG_CL_TO_STRING_HPP
