#ifndef INCG_CM_CONFIGURATION_HPP
#define INCG_CM_CONFIGURATION_HPP
#include <cstddef>

#include <array>
#include <string>

#include <tl/optional.hpp>

#include "imu.hpp"

namespace cm {
/*!
 * \brief Represents a possible configuration for
 *        the Python segmentor.
 **/
class Configuration {
public:
  friend class Builder;

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
  static const std::array<bool, 2>& skipWindowOptions() noexcept;

  /*!
   * \brief Returns the possible deleteTooClose options.
   * \return The deleteTooClose options.
   **/
  static const std::array<bool, 2>& deleteTooCloseOptions() noexcept;

  /*!
   * \brief Returns the possible deleteTooLowVariance options.
   * \return The deleteTooLowVariance options.
   **/
  static const std::array<bool, 2>& deleteTooLowVarianceOptions() noexcept;

  /*!
   * \brief Returns the possible imu options.
   * \return The imu options.
   **/
  static const std::array<Imu, 2>& imuOptions() noexcept;

  /*!
   * \brief Returns the possible segmentationKind options.
   * \return The segmentationKind options.
   **/
  static const std::array<std::string, 3>& segmentationKindOptions() noexcept;

  /*!
   * \brief Returns the possible windowSize options.
   * \return The windowSize options.
   **/
  static const std::array<std::size_t, 11>& windowSizeOptions() noexcept;

  /*!
   * \brief Returns the possible filterKind options.
   * \return The filterKind options.
   **/
  static const std::array<std::string, 2>& filterKindOptions() noexcept;

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
};
} // namespace cm
#endif // INCG_CM_CONFIGURATION_HPP
