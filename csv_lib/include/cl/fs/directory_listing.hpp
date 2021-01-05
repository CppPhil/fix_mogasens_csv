#ifndef INCG_CL_FS_DIRECTORY_LISTING_HPP
#define INCG_CL_FS_DIRECTORY_LISTING_HPP
#include <vector>

#include <cl/error.hpp>

#include <cl/fs/path.hpp>

namespace cl::fs {
/*!
 * \brief Options for directoryListing.
 **/
enum class DirectoryListingOption {
  None,               /*!< No option */
  ExcludeDotAndDotDot /*!< Exclude the . and .. directories */
};

/*!
 * \brief Creates a listing of the contents of a directory.
 * \param directoryPath The directory to list.
 * \param directoryListingOption The option to use.
 * \return A vector of the directory entries or an error on failure.
 * \warning The paths returned are not complete paths to the files,
 *          but merely the identifiers of the directory's contents.
 **/
[[nodiscard]] Expected<std::vector<Path>> directoryListing(
  const Path&            directoryPath,
  DirectoryListingOption directoryListingOption
  = DirectoryListingOption::ExcludeDotAndDotDot);
} // namespace cl::fs
#endif // INCG_CL_FS_DIRECTORY_LISTING_HPP
