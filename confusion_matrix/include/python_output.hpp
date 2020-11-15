#ifndef INCG_CM_PYTHON_OUTPUT_HPP
#define INCG_CM_PYTHON_OUTPUT_HPP
#include <cl/fs/path.hpp>

#include "configuration.hpp"

namespace cm {
/*!
 * \brief Runs the Python segmentor on `path`.
 * \param path The path to the CSV file to segment.
 * \param segmentorConfiguration The configuration to use.
 * \return The output of the Python application.
 * \throws cl::Exception if creating the process failed.
 **/
[[nodiscard]] std::string pythonOutput(
  const cl::fs::Path&  csvFilePath,
  const Configuration& segmentorConfiguration);
} // namespace cm
#endif // INCG_CM_PYTHON_OUTPUT_HPP
