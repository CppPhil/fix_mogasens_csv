#ifndef INCG_CL_DATA_POINT_HPP
#define INCG_CL_DATA_POINT_HPP
#include <iosfwd>
#include <string>

#include "cl/channel.hpp"
#include "cl/sensor.hpp"

namespace cl {
/*!
 * \brief Type to represent a data point in a data set.
 **/
class DataPoint {
public:
  /*!
   * \brief Prints a `DataPoint` to `os`.
   * \param os The ostream to print to.
   * \param dataPoint The `DataPoint` to print.
   * \return `os`
   **/
  friend std::ostream& operator<<(std::ostream& os, const DataPoint& dataPoint);

  /*!
   * \brief Creates a `DataPoint`.
   * \param fileName The file name of the CSV file that contains this
   *                 `DataPoint`.
   * \param time The time.
   * \param sensor The sensor.
   * \param channel The channel.
   * \param value The value.
   **/
  DataPoint(
    std::string fileName,
    long double time,
    Sensor      sensor,
    Channel     channel,
    long double value) noexcept;

  /*!
   * \brief Read accessor for the file name.
   * \return The file name.
   **/
  const std::string& fileName() const noexcept;

  /*!
   * \brif Read accessor for the time.
   * \return The time.
   **/
  long double time() const noexcept;

  /*!
   * \brief Read accessor for the sensor.
   * \return The sensor.
   **/
  Sensor sensor() const noexcept;

  /*!
   * \brief Read accessor for the channel.
   * \return The channel.
   **/
  Channel channel() const noexcept;

  /*!
   * \brief Read accessor for the `DataPoint` value.
   * \return The value.
   **/
  long double value() const noexcept;

private:
  std::string m_fileName;
  long double m_time;
  Sensor      m_sensor;
  Channel     m_channel;
  long double m_value;
};
} // namespace cl
#endif // INCG_CL_DATA_POINT_HPP
