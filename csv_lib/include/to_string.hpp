#ifndef INCG_FE_TO_STRING_HPP
#define INCG_FE_TO_STRING_HPP
#include <sstream>
#include <string>

namespace fe {
template<typename Ty>
inline auto to_string(const Ty& ty)
{
  std::ostringstream oss{};
  oss << ty;
  return oss.str();
}
} // namespace fe
#endif // INCG_FE_TO_STRING_HPP
