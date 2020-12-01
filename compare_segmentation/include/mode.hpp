#ifndef INCG_CS_MODE_HPP
#define INCG_CS_MODE_HPP
#include <iosfwd>

#include <cl/error.hpp>

namespace cs {
#define CS_MODE           \
  CS_MODE_X(AllDataSets)  \
  CS_MODE_X(AllPushUps)   \
  CS_MODE_X(PushUps250Hz) \
  CS_MODE_X(PushUps200Hz) \
  CS_MODE_X(Squats)

/*!
 * \brief Enumerator type for the different modes of the compare_segmentation
 *        application
 **/
enum class Mode {
#define CS_MODE_X(enm) enm,
  CS_MODE
#undef CS_MODE_X
};

/*!
 * \brief Prints a `Mode` to an ostream.
 * \param os The ostream to print to.
 * \param mode The `Mode` to print.
 * \return `os`
 **/
std::ostream& operator<<(std::ostream& os, Mode mode);

/*!
 * \brief Parses a null-terminated byte character string as a `Mode`.
 * \param szCmdArg A null-terminated byte character string containing
 *                 a textual representation of a `Mode`.
 * \return The mode parsed out of `szCmdArg` or an error.
 **/
[[nodiscard]] cl::Expected<Mode> parseMode(const char* szCmdArg);
} // namespace cs
#endif // INCG_CS_MODE_HPP
