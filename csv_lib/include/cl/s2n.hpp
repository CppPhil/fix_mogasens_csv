#ifndef INCG_CL_S2N_HPP
#define INCG_CL_S2N_HPP
#include <cassert>
#include <cstddef>

#include <string>
#include <type_traits>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "error.hpp"

namespace cl {
template<typename Integer>
inline Expected<Integer> s2n(
  const std::string&   str,
  std::size_t*         pos  = nullptr,
  [[maybe_unused]] int base = 10)
{
  try {
    if constexpr (std::is_same_v<Integer, int>) {
      return std::stoi(str, pos, base);
    }
    else if constexpr (std::is_same_v<Integer, long>) {
      return std::stol(str, pos, base);
    }
    else if constexpr (std::is_same_v<Integer, long long>) {
      return std::stoll(str, pos, base);
    }
    else if constexpr (std::is_same_v<Integer, unsigned long>) {
      return std::stoul(str, pos, base);
    }
    else if constexpr (std::is_same_v<Integer, unsigned long long>) {
      return std::stoull(str, pos, base);
    }
    else if constexpr (std::is_same_v<Integer, float>) {
      return std::stof(str, pos);
    }
    else if constexpr (std::is_same_v<Integer, double>) {
      return std::stod(str, pos);
    }
    else if constexpr (std::is_same_v<Integer, long double>) {
      return std::stold(str, pos);
    }
    else {
      assert(false && "Invalid type given to s2n!");
      return CL_UNEXPECTED(
        Error::InvalidArgument, "Invalid type given to s2n!");
    }
  }
  catch (const std::invalid_argument& ex) {
    return CL_UNEXPECTED(
      Error::InvalidArgument,
      fmt::format("s2n: caught invalid_argument: \"{}\"", ex.what()));
  }
  catch (const std::out_of_range& ex) {
    return CL_UNEXPECTED(
      Error::OutOfRange,
      fmt::format("s2n: caught out_of_range: \"{}\"", ex.what()));
  }
}
} // namespace cl
#endif // INCG_CL_S2N_HPP
