#include "confusion_matrix.hpp"

namespace cm {
ConfusionMatrix::ConfusionMatrix()
  : m_truePositives{0}
  , m_trueNegatives{0}
  , m_falsePositives{0}
  , m_falseNegatives{0}
  , m_totalCount{0}
{
}

std::uint64_t ConfusionMatrix::truePositives() const noexcept
{
  return m_truePositives;
}

std::uint64_t ConfusionMatrix::trueNegatives() const noexcept
{
  return m_trueNegatives;
}

std::uint64_t ConfusionMatrix::falsePositives() const noexcept
{
  return m_falsePositives;
}

std::uint64_t ConfusionMatrix::falseNegatives() const noexcept
{
  return m_falseNegatives;
}

std::uint64_t ConfusionMatrix::totalCount() const noexcept
{
  return m_totalCount;
}

ConfusionMatrix& ConfusionMatrix::incrementTruePositives() noexcept
{
  ++m_truePositives;
  ++m_totalCount;
  return *this;
}

ConfusionMatrix& ConfusionMatrix::incrementTrueNegatives() noexcept
{
  ++m_trueNegatives;
  ++m_totalCount;
  return *this;
}

ConfusionMatrix& ConfusionMatrix::incrementFalsePositives() noexcept
{
  ++m_falsePositives;
  ++m_totalCount;
  return *this;
}

ConfusionMatrix& ConfusionMatrix::incrementFalseNegatives() noexcept
{
  ++m_falseNegatives;
  ++m_totalCount;
  return *this;
}

ConfusionMatrix& ConfusionMatrix::operator+=(
  const ConfusionMatrix& other) noexcept
{
  m_truePositives += other.m_truePositives;
  m_trueNegatives += other.m_trueNegatives;
  m_falsePositives += other.m_falsePositives;
  m_falseNegatives += other.m_falseNegatives;
  m_totalCount += other.m_totalCount;

  return *this;
}
} // namespace cm
