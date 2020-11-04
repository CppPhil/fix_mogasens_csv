#ifndef INCG_CS_CSV_LINE_HPP
#define INCG_CS_CSV_LINE_HPP
#include <cstdint>

#include <string>
#include <vector>

#include <tl/optional.hpp>

#include "filter_kind.hpp"
#include "segmentation_kind.hpp"

namespace cs {
/*!
 * \brief Builder for a CSV line.
 *
 * Builder type for a CSV line.
 * All write accessors have to be called before the build member function is
 * called!
 **/
class CsvLineBuilder {
public:
  using this_type = CsvLineBuilder;

  /*!
   * \brief Creates an empty, invalid CsvLineBuilder.
   **/
  CsvLineBuilder();

  /*!
   * \brief Write accessor for the skip window property.
   * \param value The value to use.
   * \return *this
   **/
  this_type& skipWindow(bool value);

  /*!
   * \brief Write accessor for the delete too close property.
   * \param value The value to use.
   * \return *this
   **/
  this_type& deleteTooClose(bool value);

  /*!
   * \brief Write accessor for the delete low variance property.
   * \param value The value to use.
   * \return *this
   **/
  this_type& deleteLowVariance(bool value);

  /*!
   * \brief Write accessor for the kind property.
   * \param value The value to use.
   * \return *this
   **/
  this_type& kind(SegmentationKind value);

  /*!
   * \brief Write accessor for the window size property.
   * \param value The value to use.
   * \return *this
   **/
  this_type& windowSize(std::uint64_t value);

  /*!
   * \brief Write accessor for the filter property.
   * \param value The value to use.
   * \return *this
   **/
  this_type& filter(FilterKind value);

  /*!
   * \brief Write accessor for the data set property.
   * \param value The value to use.
   * \return *this
   **/
  this_type& dataSet(std::string value);

  /*!
   * \brief Write accessor for the sensor property.
   * \param value The value to use.
   * \return *this
   **/
  this_type& sensor(std::uint64_t value);

  /*!
   * \brief Write accessor for the repetitions property.
   * \param value The value to use.
   * \return *this
   **/
  this_type& repetitions(std::uint64_t value);

  /*!
   * \brief Write accessor for the segmentation points property.
   * \param value The value to use.
   * \return *this
   **/
  this_type& segmentationPoints(std::uint64_t value);

  /*!
   * \brief Write accessor for the is old property.
   * \param value The value to use.
   * \return *this
   **/
  this_type& isOld(bool value);

  /*!
   * \brief Builds the CSV line as a vector containing the cells of the CSV
   *        line.
   * \return The resulting vector of strings.
   * \warning May only be called
   *          after all the write accessors have been called.
   **/
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
  tl::optional<std::uint64_t>    m_repetitions;
  tl::optional<std::uint64_t>    m_segmentationPoints;
  tl::optional<bool>             m_isOld;
};
} // namespace cs
#endif // INCG_CS_CSV_LINE_HPP
