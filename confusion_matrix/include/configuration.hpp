#ifndef INCG_CM_CONFIGURATION_HPP
#define INCG_CM_CONFIGURATION_HPP
#include <cstddef>

#include <deque>
#include <iosfwd>
#include <string>
#include <vector>

#include <tl/optional.hpp>

#include <pl/hash.hpp>
#include <pl/total_order.hpp>

#include <cl/fs/path.hpp>

#include "imu.hpp"

namespace cm {
class Configuration;
} // namespace cm

namespace std {
template<>
struct hash<::cm::Configuration>;
} // namespace std

namespace cm {
/*!
 * \brief Represents a possible configuration for
 *        the Python segmentor.
 **/
class Configuration {
public:
  friend class Builder;
  friend struct std::hash<Configuration>;

  /*!
   * \brief Builder type for `Configuration`.
   **/
  class Builder {
  public:
    /*!
     * \brief Creates an empty `Builder`.
     **/
    Builder() noexcept;

    /*!
     * \brief Sets the skipWindow property.
     * \param value The value to use.
     * \return *this
     **/
    Builder& skipWindow(bool value);

    /*!
     * \brief Sets the deleteTooClose property.
     * \param value The value to use.
     * \return *this
     **/
    Builder& deleteTooClose(bool value);

    /*!
     * \brief Sets the deleteTooLowVariance property.
     * \param value The value to use.
     * \return *this
     **/
    Builder& deleteTooLowVariance(bool value);

    /*!
     * \brief Sets the imu property.
     * \param value The value to use.
     * \return *this
     **/
    Builder& imu(Imu value);

    /*!
     * \brief Sets the segmentationKind property.
     * \param value The value to use.
     * \return *this
     **/
    Builder& segmentationKind(std::string value);

    /*!
     * \brief Sets the windowSize property.
     * \param value The value to use.
     * \return *this
     **/
    Builder& windowSize(std::size_t value);

    /*!
     * \brief Sets the filterKind property.
     * \param value The value to use.
     * \return *this
     **/
    Builder& filterKind(std::string value);

    /*!
     * \brief Builds a `Configuration`.
     * \return The `Configuration` built.
     * \throws cl::Exception if one of the properties has not been set
     *                       or is invalid.
     **/
    Configuration build() const;

  private:
    tl::optional<bool>        m_skipWindow;
    tl::optional<bool>        m_deleteTooClose;
    tl::optional<bool>        m_deleteTooLowVariance;
    tl::optional<Imu>         m_imu;
    tl::optional<std::string> m_segmentationKind;
    tl::optional<std::size_t> m_windowSize;
    tl::optional<std::string> m_filterKind;
  };

  /*!
   * \brief Returns the possible skipWindow options.
   * \return The skipWindow options.
   **/
  static const std::deque<bool>& skipWindowOptions() noexcept;

  /*!
   * \brief Returns the possible deleteTooClose options.
   * \return The deleteTooClose options.
   **/
  static const std::deque<bool>& deleteTooCloseOptions() noexcept;

  /*!
   * \brief Returns the possible deleteTooLowVariance options.
   * \return The deleteTooLowVariance options.
   **/
  static const std::deque<bool>& deleteTooLowVarianceOptions() noexcept;

  /*!
   * \brief Returns the possible imu options.
   * \return The imu options.
   **/
  static const std::vector<Imu>& imuOptions() noexcept;

  /*!
   * \brief Returns the possible segmentationKind options.
   * \return The segmentationKind options.
   **/
  static const std::vector<std::string>& segmentationKindOptions() noexcept;

  /*!
   * \brief Returns the possible windowSize options.
   * \return The windowSize options.
   **/
  static const std::vector<std::size_t>& windowSizeOptions() noexcept;

  /*!
   * \brief Returns the possible filterKind options.
   * \return The filterKind options.
   **/
  static const std::vector<std::string>& filterKindOptions() noexcept;

  /*!
   * \brief Compares two Configurations for equality.
   * \param lhs The first operand.
   * \param rhs The second operand.
   * \return true if `lhs` and `rhs` are considered to be equal.
   **/
  friend bool operator==(
    const Configuration& lhs,
    const Configuration& rhs) noexcept;

  /*!
   * \brief Less than compares two Configurations.
   * \param lhs The first operand.
   * \param rhs The second operand.
   * \return true if `lhs` is considered less than `rhs`; otherwise false.
   **/
  friend bool operator<(
    const Configuration& lhs,
    const Configuration& rhs) noexcept;

  /*!
   * \brief Prints `config` to `os`.
   * \param os The ostream to print to.
   * \param config The `Configuration` to print.
   * \return `os`
   **/
  friend std::ostream& operator<<(
    std::ostream&        os,
    const Configuration& config);

  /*!
   * \brief Default constructor.
   * \warning This constructor is only there to work around Microsoft
   *          buggedness, don't use.
   * \note Creates an uninitialized object!
   **/
  Configuration();

  /*!
   * \brief Read accessor for the skipWindow property.
   * \return The skipWindow option.
   **/
  [[nodiscard]] bool skipWindow() const noexcept;

  /*!
   * \brief Read accessor for the deleteTooClose property.
   * \return The deleteTooClose option.
   **/
  [[nodiscard]] bool deleteTooClose() const noexcept;

  /*!
   * \brief Read accessor for the deleteTooLowVariance property.
   * \return The deleteTooLowVariance option.
   **/
  [[nodiscard]] bool deleteTooLowVariance() const noexcept;

  /*!
   * \brief Read accessor for the imu property.
   * \return The imu option.
   **/
  [[nodiscard]] Imu imu() const noexcept;

  /*!
   * \brief Read accessor for the segmentationKind property.
   * \return The segmentationKind option.
   **/
  [[nodiscard]] const std::string& segmentationKind() const noexcept;

  /*!
   * \brief Read accessor for the windowSize property.
   * \return The windowSize option.
   **/
  [[nodiscard]] std::size_t windowSize() const noexcept;

  /*!
   * \brief Read accessor for the filterKind property.
   * \return The filterKind option.
   **/
  [[nodiscard]] const std::string& filterKind() const noexcept;

  /*!
   * \brief Checks if this object is initialized and thus valid.
   * \return true if this object is initialized; false otherwise.
   * \warning If you use the Builder to construct (as you should)
   *          this member function will always return true.
   *          false will only be returned if you use the (invalid)
   *          default constructor that servers as a workaround
   *          for MSVC bugs.
   **/
  [[nodiscard]] bool isInitialized() const noexcept;

  /*!
   * \brief Create a file path for this kind of `Configuration`.
   * \return The file path for this kind of `Configuration`.
   **/
  [[nodiscard]] cl::fs::Path createFilePath() const;

  /*!
   * \brief Serializes a map of segmentation points to the file path for this
   *        `Configuration`.
   * \param segmentationPointsMap The map to serialize.
   * \return true on success; false otherwise.
   **/
  [[nodiscard]] bool serializeSegmentationPoints(
    const std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>&
      segmentationPointsMap) const;

  /*!
   * \brief Imports segmentation points from the file path for this
   *        `Configuration`.
   * \return The imported segmentation points.
   * \throws cl::Exception if the file path for this `Configuration` does not
   *                       exist or an error occurs while reading / parsing.
   **/
  [[nodiscard]] std::unordered_map<cl::fs::Path, std::vector<std::uint64_t>>
  importSegmentationPoints() const;

private:
  Configuration(
    bool        skipWindow,
    bool        deleteTooClose,
    bool        deleteTooLowVariance,
    Imu         imu,
    std::string segmentationKind,
    std::size_t windowSize,
    std::string filterKind);

  bool        m_skipWindow;
  bool        m_deleteTooClose;
  bool        m_deleteTooLowVariance;
  Imu         m_imu;
  std::string m_segmentationKind;
  std::size_t m_windowSize;
  std::string m_filterKind;
  bool        m_isInitialized;
};

// Generate the other comparison operators. (Requires operator== and operator<)
PL_TOTAL_ORDER(Configuration)
} // namespace cm

// Make the type hashable.
namespace std {
template<>
struct hash<::cm::Configuration> {
  size_t operator()(const ::cm::Configuration& configuration) const
  {
    // The classic hash combine.
    return ::pl::hash(
      configuration.m_skipWindow,
      configuration.m_deleteTooClose,
      configuration.m_deleteTooLowVariance,
      configuration.m_imu,
      configuration.m_segmentationKind,
      configuration.m_windowSize,
      configuration.m_filterKind);
  }
};
} // namespace std
#endif // INCG_CM_CONFIGURATION_HPP
