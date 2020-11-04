#ifndef INCG_CS_PATHS_HPP
#define INCG_CS_PATHS_HPP
#include <pl/string_view.hpp>

namespace cs {
/*!
 * \brief Relative path to the directory containing the preprocessed log files.
 **/
inline constexpr pl::string_view logPath{"segmentation_comparison/logs"};

/*!
 * \brief Relative path to the directory containing the old log files.
 **/
inline constexpr pl::string_view oldLogPath{"segmentation_comparison/logs/old"};
} // namespace cs
#endif // INCG_CS_PATHS_HPP
