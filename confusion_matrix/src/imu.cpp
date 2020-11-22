#include <ostream>

#include <pl/algo/ranged_algorithms.hpp>
#include <pl/unreachable.hpp>

#include "imu.hpp"

namespace cm {
namespace {
/*!
 * \brief Turns an ASCII character into lower case.
 * \param inputCharacter The ASCII character get the lower case variant for.
 * \return The lower case ASCII character of `inputCharacter`.
 **/
char toLower(char inputCharacter)
{
  unsigned char character{static_cast<unsigned char>(inputCharacter)};
  character |= 0b0010'0000; /* Make it lower case */
  return static_cast<char>(character);
}

/*!
 * \brief Creates a lower case ASCII string of `string`.
 * \param string An ASCII string to lower case.
 * \return The lower case variant of `string`.
 **/
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
