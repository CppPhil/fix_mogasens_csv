#include <cstring>

#include <ostream>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/unreachable.hpp>

#include "mode.hpp"

namespace cs {
std::ostream& operator<<(std::ostream& os, Mode mode)
{
  switch (mode) {
#define CS_MODE_X(enm) \
  case Mode::enm: return os << #enm;
    CS_MODE
#undef CS_MODE_X
  }

  PL_UNREACHABLE();
}

cl::Expected<Mode> parseMode(const char* szCmdArg)
{
#define CS_MODE_X(enm) \
  if (std::strcmp(#enm, szCmdArg) == 0) { return Mode::enm; }
  CS_MODE
#undef CS_MODE_X

  return CL_UNEXPECTED(
    cl::Error::Parsing,
    fmt::format(
      "\"{}\" could not be parsed as a valid cs::Mode enumerator!", szCmdArg));
}
} // namespace cs
