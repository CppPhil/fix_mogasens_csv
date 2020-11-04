#include <pl/algo/erase.hpp>
#include <pl/algo/ranged_algorithms.hpp>

#include <cl/fs/directory_listing.hpp>

#include "log_files.hpp"

namespace cs {
cl::Expected<std::vector<cl::fs::Path>> logFiles(pl::string_view directoryPath)
{
  cl::Expected<std::vector<cl::fs::Path>> expectedListing{
    cl::fs::directoryListing(cl::fs::Path{directoryPath.to_string()})};

  if (!expectedListing.has_value()) {
    return tl::make_unexpected(expectedListing.error());
  }

  std::vector<cl::fs::Path> listing{*std::move(expectedListing)};

  // Disregard files that aren't logs.
  pl::algo::erase_if(listing, [](const cl::fs::Path& currentPath) {
    const pl::string_view stringView{currentPath.str()};
    return !stringView.ends_with(".log");
  });

  // Prepend the path to the directory to the file paths, because
  // directoryListing doesn't include them.
  pl::algo::transform(
    listing, listing.begin(), [directoryPath](const cl::fs::Path& path) {
      return cl::fs::Path{directoryPath.to_string() + "/" + path.str()};
    });

  return listing;
}
} // namespace cs
