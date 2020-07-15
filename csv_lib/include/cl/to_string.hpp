#ifndef INCG_CL_TO_STRING_HPP
#define INCG_CL_TO_STRING_HPP
#include <sstream>
#include <string>

namespace cl {
template<typename Ty>
inline auto to_string(const Ty& ty)
{
  std::ostringstream oss{};
  oss << ty;
  return oss.str();
}
} // namespace cl
#endif // INCG_CL_TO_STRING_HPP
