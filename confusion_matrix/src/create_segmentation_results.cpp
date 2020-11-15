#include "create_segmentation_results.hpp"
#include "segment.hpp"

namespace cm {
std::unordered_map<
  Configuration,
  std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>
createSegmentationResults()
{
  std::unordered_map<
    Configuration,
    std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>>
    segmentationResults{};

  for (bool skipWindowOption : Configuration::skipWindowOptions()) {
    for (bool deleteTooCloseOption : Configuration::deleteTooCloseOptions()) {
      for (bool deleteTooLowVarianceOption :
           Configuration::deleteTooLowVarianceOptions()) {
        for (Imu imuOption : Configuration::imuOptions()) {
          for (const std::string& segmentationKindOption :
               Configuration::segmentationKindOptions()) {
            for (std::size_t windowSizeOption :
                 Configuration::windowSizeOptions()) {
              for (const std::string& filterKindOption :
                   Configuration::filterKindOptions()) {
                const Configuration configuration{
                  Configuration::Builder{}
                    .skipWindow(skipWindowOption)
                    .deleteTooClose(deleteTooCloseOption)
                    .deleteTooLowVariance(deleteTooLowVarianceOption)
                    .imu(imuOption)
                    .segmentationKind(segmentationKindOption)
                    .windowSize(windowSizeOption)
                    .filterKind(filterKindOption)
                    .build()};
                segmentationResults[configuration] = segment(configuration);
              }
            }
          }
        }
      }
    }
  }

  return segmentationResults;
}
} // namespace cm
