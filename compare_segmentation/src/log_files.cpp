#include <pl/algo/erase.hpp>

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

  pl::algo::erase_if(listing, [](const cl::fs::Path& currentPath) {
    const pl::string_view stringView{currentPath.str()};
    return !stringView.ends_with(".log");
  });

  return listing;
}
} // namespace cs
