#ifndef INCG_CM_CONFUSION_MATRIX_HPP
#define INCG_CM_CONFUSION_MATRIX_HPP
#include <cstdint>

namespace cm {
/*!
 * \brief Type to represent a confusion matrix.
 **/
class ConfusionMatrix {
public:
  using this_type = ConfusionMatrix;

  /*!
   * \brief Default constructs a `ConfusionMatrix` initializing all data members
   *        with 0.
   **/
  ConfusionMatrix();

  /*!
   * \brief Read accessor for the true positive count.
   * \return The true positive count.
   **/
  [[nodiscard]] std::uint64_t truePositives() const noexcept;

  /*!
   * \brief Read accessor for the true negative count.
   * \return The true negative count.
   **/
  [[nodiscard]] std::uint64_t trueNegatives() const noexcept;

  /*!
   * \brief Read accessor for the false positive count.
   * \return The false positive count.
   **/
  [[nodiscard]] std::uint64_t falsePositives() const noexcept;

  /*!
   * \brief Read accessor for the false negative count.
   * \return The false negative count.
   **/
  [[nodiscard]] std::uint64_t falseNegatives() const noexcept;

  /*!
   * \brief Read accessor for the total count.
   * \return The total count.
   **/
  [[nodiscard]] std::uint64_t totalCount() const noexcept;

  /*!
   * \brief Increments the true positive count and the total count
   * \return *this
   **/
  this_type& incrementTruePositives() noexcept;

  /*!
   * \brief Increments the true negative count and the total count
   * \return *this
   **/
  this_type& incrementTrueNegatives() noexcept;

  /*!
   * \brief Increments the false positive count and the total count
   * \return *this
   **/
  this_type& incrementFalsePositives() noexcept;

  /*!
   * \brief Increments the false negative count and the total count
   * \return *this
   **/
  this_type& incrementFalseNegatives() noexcept;

  /*!
   * \brief Accumulates `other` into *this.
   * \param other The other `ConfusionMatrix` to add to this `ConfusionMatrix`.
   * \return *this
   **/
  this_type& operator+=(const ConfusionMatrix& other) noexcept;

private:
  std::uint64_t m_truePositives; /*!< If a given fake one exists within the
                                  *   range of a real one.
                                  **/

  std::uint64_t m_trueNegatives; /*!< If there's no real one and no fake one for
                                  *   that as well.
                                  **/

  std::uint64_t m_falsePositives; /*!< If a given fake one is not within the
                                   *   range of any real one.
                                   **/

  std::uint64_t m_falseNegatives; /*!< If for a given real one there is no fake
                                   *   one to be found in its range.
                                   **/

  std::uint64_t m_totalCount; /*!< How often any value was incremented. */
};
} // namespace cm
#endif // INCG_CM_CONFUSION_MATRIX_HPP
