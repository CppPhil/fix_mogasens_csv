#include <ostream>

#include <pl/algo/ranged_algorithms.hpp>
#include <pl/unreachable.hpp>

#include "imu.hpp"

namespace cm {
namespace {
char toLower(char inputCharacter)
{
  unsigned char character{static_cast<unsigned char>(inputCharacter)};
  character |= 0b0010'0000;
  return static_cast<char>(character);
}

std::string toLower(std::string string)
{
  pl::algo::transform(
    string, string.begin(), [](char c) { return toLower(c); });
  return string;
}
} // namespace

std::ostream& operator<<(std::ostream& os, Imu imu)
{
  switch (imu) {
#define CM_IMU_X(enm) \
  case Imu::enm: return os << toLower(#enm);
    CM_IMU
#undef CM_IMU_X
  }

  PL_UNREACHABLE();
}
} // namespace cm
