#!/usr/bin/env python3
# -*- coding: utf8 -*-

# Sensors
# 769 = left arm
# 770 = belly
# 771 = right arm
# 772 = chest

import argparse
import csv
import sys

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd


def sensor_to_string(sensor_id):
  left_arm_sensor_id = 769
  belly_sensor_id = 770
  right_arm_sensor_id = 771
  chest_sensor_id = 772

  if sensor_id == left_arm_sensor_id:
    return "left arm sensor"
  elif sensor_id == belly_sensor_id:
    return "belly sensor"
  elif sensor_id == right_arm_sensor_id:
    return "right arm sensor"
  elif sensor_id == chest_sensor_id:
    return "chest sensor"
  else:
    return f"bogus sensor, id: {sensor_id}"


def ticks(values, step):
  # + 1 because np.arange works with half open ranges, but we want a closed range.
  return np.arange(min(values), max(values) + 1, step)


def plot(imu, dataframe):
  first_color = 'blue'
  second_color = 'red'
  third_color = 'green'

  first_label = 'x'
  second_label = 'y'
  third_label = 'z'

  linewidth = 0.6

  plot_channel = lambda channel, color, label: plt.plot('time',
                                                        channel,
                                                        data=dataframe,
                                                        color=color,
                                                        label=label,
                                                        linewidth=linewidth)

  if imu == 'accelerometer':
    plot_channel('channel1', first_color, first_label)
    plot_channel('channel2', second_color, second_label)
    plot_channel('channel3', third_color, third_label)
  elif imu == 'gyroscope':
    plot_channel('channel4', first_color, first_label)
    plot_channel('channel5', second_color, second_label)
    plot_channel('channel6', third_color, third_label)
  else:
    print(
        f"imu was \"{imu}\" which is neither \"accelerometer\" nor \"gyroscope\", exiting.",
        file=sys.stderr)
    sys.exit()


def imu_unit(imu):
  if imu == 'accelerometer':
    return 'g'
  elif imu == 'gyroscope':
    return 'deg/s'
  else:
    return f"invalid imu: \"{imu}\"!"


if __name__ == "__main__":
  parser = argparse.ArgumentParser(description='Plot MoGaSens CSV file.')
  parser.add_argument('csv_file_path',
                      type=str,
                      help='Path to the CSV file to plot.')
  parser.add_argument('sensor',
                      type=int,
                      help='The sensor to plot (769 | 770 | 771 | 772).')
  parser.add_argument(
      'imu',
      type=str,
      help='The Inertial Measurement Unit to use (accelerometer / gyroscope).')

  args = parser.parse_args()

  csv_file_path = args.csv_file_path
  desired_sensor = args.sensor
  imu = args.imu

  if sensor_to_string(desired_sensor).startswith('bogus sensor'):
    print(f"{desired_sensor} is not a valid sensor ID, exiting.",
          file=sys.stderr)
    sys.exit()

  time_column_index = 0
  # We don't care about the hardware timestamp.
  extract_id_column_index = 2
  # We don't care about the trigger (it's always 0).
  accelerometer_x_column_index = 4
  accelerometer_y_column_index = 5
  accelerometer_z_column_index = 6
  gyroscope_x_column_index = 7
  gyroscope_y_column_index = 8
  gyroscope_z_column_index = 9

  time = []
  extract_id = []
  accelerometer_x = []
  accelerometer_y = []
  accelerometer_z = []
  gyroscope_x = []
  gyroscope_y = []
  gyroscope_z = []

  with open(csv_file_path, 'r') as csv_file:
    plots = csv.reader(csv_file, delimiter=',')
    for row_count, row in enumerate(plots):
      if row_count == 0:  # Skip the header row
        continue

      time.append(float(row[time_column_index]))
      extract_id.append(int(row[extract_id_column_index]))
      accelerometer_x.append(float(row[accelerometer_x_column_index]))
      accelerometer_y.append(float(row[accelerometer_y_column_index]))
      accelerometer_z.append(float(row[accelerometer_z_column_index]))
      gyroscope_x.append(float(row[gyroscope_x_column_index]))
      gyroscope_y.append(float(row[gyroscope_y_column_index]))
      gyroscope_z.append(float(row[gyroscope_z_column_index]))

  time_data = []
  channel1_data = []
  channel2_data = []
  channel3_data = []
  channel4_data = []
  channel5_data = []
  channel6_data = []

  for i in range(len(time)):
    current_sensor_id = extract_id[i]

    if current_sensor_id == desired_sensor:
      time_data.append(time[i])
      channel1_data.append(accelerometer_x[i])
      channel2_data.append(accelerometer_y[i])
      channel3_data.append(accelerometer_z[i])
      channel4_data.append(gyroscope_x[i])
      channel5_data.append(gyroscope_y[i])
      channel6_data.append(gyroscope_z[i])

  df = pd.DataFrame({
      'time': time_data,
      'channel1': channel1_data,
      'channel2': channel2_data,
      'channel3': channel3_data,
      'channel4': channel4_data,
      'channel5': channel5_data,
      'channel6': channel6_data
  })

  title = f"{csv_file_path}_{sensor_to_string(desired_sensor)}_{imu}".replace(
      " ", "_")
  png_file = f"{title}.png"

  plot(imu, df)
  plt.legend(loc='upper right')

  plt.title(title)
  plt.ylabel(imu_unit(imu))
  plt.xlabel('time (in seconds)')
  plt.xticks(ticks(values=time_data, step=5))  # Steps of 5 seconds.
  plt.grid()
  plt.savefig(png_file, bbox_inches='tight')

  print(f"Wrote \"{png_file}\".")
