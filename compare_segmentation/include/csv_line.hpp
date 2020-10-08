#ifndef INCG_CS_CSV_LINE_HPP
#define INCG_CS_CSV_LINE_HPP
#include <cstdint>

#include <string>
#include <vector>

#include <tl/optional.hpp>

#include "filter_kind.hpp"
#include "segmentation_kind.hpp"

namespace cs {
class CsvLineBuilder {
public:
  using this_type = CsvLineBuilder;

  CsvLineBuilder();

  this_type& skipWindow(bool value);

  this_type& deleteTooClose(bool value);

  this_type& deleteLowVariance(bool value);

  this_type& kind(SegmentationKind value);

  this_type& windowSize(std::uint64_t value);

  this_type& filter(FilterKind value);

  this_type& dataSet(std::string value);

  this_type& sensor(std::uint64_t value);

  this_type& pushUps(std::uint64_t value);

  this_type& segmentationPoints(std::uint64_t value);

  [[nodiscard]] std::vector<std::string> build() const;

private:
  tl::optional<bool>             m_skipWindow;
  tl::optional<bool>             m_deleteTooClose;
  tl::optional<bool>             m_deleteLowVariance;
  tl::optional<SegmentationKind> m_kind;
  tl::optional<std::uint64_t>    m_windowSize;
  tl::optional<FilterKind>       m_filter;
  tl::optional<std::string>      m_dataSet;
  tl::optional<std::uint64_t>    m_sensor;
  tl::optional<std::uint64_t>    m_pushUps;
  tl::optional<std::uint64_t>    m_segmentationPoints;
};
} // namespace cs
#endif // INCG_CS_CSV_LINE_HPP
