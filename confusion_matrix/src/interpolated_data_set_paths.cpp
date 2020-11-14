#include <string>

#include <pl/algo/erase.hpp>
#include <pl/algo/ranged_algorithms.hpp>
#include <pl/string_view.hpp>

#include <cl/exception.hpp>
#include <cl/fs/directory_listing.hpp>

#include "interpolated_data_set_paths.hpp"

namespace cm {
std::vector<cl::fs::Path> interpolatedDataSetPaths()
{
  static const std::string relativeInterpolatedPath{
    "resources/preprocessed/Interpolated"};

  cl::Expected<std::vector<cl::fs::Path>> expectedPaths{
    cl::fs::directoryListing(relativeInterpolatedPath)};

  if (!expectedPaths.has_value()) { expectedPaths.error().raise(); }

  std::vector<cl::fs::Path> paths{*std::move(expectedPaths)};

  pl::algo::erase_if(paths, [](const cl::fs::Path& currentPath) {
    const pl::string_view path{currentPath.str()};
    return !path.ends_with(".csv") || path.contains("Falschausführung")
           || path.contains("Idle") || path.contains("Plank")
           || path.contains("with_repetition_ids");
  });

  if (paths.empty()) { CL_THROW("No paths were found!"); }

  pl::algo::transform(paths, paths.begin(), [](const cl::fs::Path& path) {
    return cl::fs::Path{relativeInterpolatedPath + "/" + path.str()};
  });

  return paths;
}
} // namespace cm
