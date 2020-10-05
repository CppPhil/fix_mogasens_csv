#ifndef INCG_CL_FS_DIRECTORY_LISTING_HPP
#define INCG_CL_FS_DIRECTORY_LISTING_HPP
#include <vector>

#include <cl/error.hpp>

#include <cl/fs/path.hpp>

namespace cl::fs {
enum class DirectoryListingOption { None, ExcludeDotAndDotDot };

[[nodiscard]] Expected<std::vector<Path>> directoryListing(
  const Path&            directoryPath,
  DirectoryListingOption directoryListingOption
  = DirectoryListingOption::ExcludeDotAndDotDot);
} // namespace cl::fs
#endif // INCG_CL_FS_DIRECTORY_LISTING_HPP
