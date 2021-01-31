#ifndef INCG_CS_PATHS_HPP
#define INCG_CS_PATHS_HPP
#include <pl/string_view.hpp>

namespace cs {
/*!
 * \brief Relative path to the directory containing the preprocessed log files.
 * \note The working directory is assumed to be the root mogasens_csv path.
 *       The bash / batch scripts should already take care of that.
 **/
inline constexpr pl::string_view logPath{"segmentation_comparison/logs"};

/*!
 * \brief Relative path to the directory containing the old log files.
 * \note The working directory is assumed to be the root mogasens_csv path.
 *       The bash / batch scripts should already take care of that.
 **/
inline constexpr pl::string_view oldLogPath{"segmentation_comparison/logs/old"};
} // namespace cs
#endif // INCG_CS_PATHS_HPP
