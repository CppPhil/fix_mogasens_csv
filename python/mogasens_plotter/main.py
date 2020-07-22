#!/usr/bin/env python3
# -*- coding: utf8 -*-

# Sensors
# 769 = left arm
# 770 = belly
# 771 = right arm
# 772 = chest

import argparse
import sys

import matplotlib.pyplot as plt
import pandas as pd

from constants import *
from data_set import DataSet
from imu_unit import imu_unit
from moving_average_filter import moving_average_filter
from sensor_to_string import sensor_to_string


def plot(the_imu, data_frame):
  first_color = 'blue'
  second_color = 'red'
  third_color = 'green'

  first_label = 'x'
  second_label = 'y'
  third_label = 'z'

  line_width = 0.6

  def plot_channel(channel, color, label):
    plt.plot(hardware_timestamp_string(),
             channel,
             data=data_frame,
             color=color,
             label=label,
             linewidth=line_width)

  if the_imu == accelerometer_string():
    plot_channel(channel1_string(), first_color, first_label)
    plot_channel(channel2_string(), second_color, second_label)
    plot_channel(channel3_string(), third_color, third_label)
  elif the_imu == gyroscope_string():
    plot_channel(channel4_string(), first_color, first_label)
    plot_channel(channel5_string(), second_color, second_label)
    plot_channel(channel6_string(), third_color, third_label)
  else:
    print(
        f'imu was "{the_imu}" which is neither "#{accelerometer_string()}" nor "#{gyroscope_string()}", '
        f'exiting.',
        file=sys.stderr)
    sys.exit()


def select_filter(use_moving_average_filter):
  if use_moving_average_filter:
    return moving_average_filter
  return lambda sample_count, data: data


def main():
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
  parser.add_argument('--moving_average_filter',
                      dest='moving_average_filter',
                      action='store_true')
  parser.add_argument('--no-moving_average_filter',
                      dest='moving_average_filter',
                      action='store_false')
  parser.set_defaults(moving_average_filter=False)
  parser.add_argument('moving_average_filter_sample_count',
                      type=int,
                      help='The sample count to use')

  args = parser.parse_args()

  csv_file_path = args.csv_file_path
  desired_sensor = args.sensor
  imu = args.imu
  use_moving_average_filter = args.moving_average_filter
  moving_average_filter_sample_count = args.moving_average_filter_sample_count

  if sensor_to_string(desired_sensor).startswith('bogus sensor'):
    print(f"{desired_sensor} is not a valid sensor ID, exiting.",
          file=sys.stderr)
    sys.exit()

  data_set = DataSet\
      .from_file(csv_file_path)\
      .filter_by_sensor(desired_sensor)\
      .apply_filter(lambda data: select_filter(use_moving_average_filter)(moving_average_filter_sample_count, data))

  filter_kind = "no_filter"

  if use_moving_average_filter:
    filter_kind = f"avg_filter_{moving_average_filter_sample_count}"

  hardware_timestamp_data = []
  channel1_data = []
  channel2_data = []
  channel3_data = []
  channel4_data = []
  channel5_data = []
  channel6_data = []

  hardware_timestamp_accumulator = []
  channel1_accumulator = []
  channel2_accumulator = []
  channel3_accumulator = []
  channel4_accumulator = []
  channel5_accumulator = []
  channel6_accumulator = []

  hardware_timestamp_threshold_offset = 10000  # 10 seconds
  hardware_timestamp_threshold = data_set.hardware_timestamp[
      0] + hardware_timestamp_threshold_offset

  def append_accumulator(data, accumulator):
    data.append(accumulator.copy())
    accumulator.clear()

  def append_accumulators():
    append_accumulator(hardware_timestamp_data, hardware_timestamp_accumulator)
    append_accumulator(channel1_data, channel1_accumulator)
    append_accumulator(channel2_data, channel2_accumulator)
    append_accumulator(channel3_data, channel3_accumulator)
    append_accumulator(channel4_data, channel4_accumulator)
    append_accumulator(channel5_data, channel5_accumulator)
    append_accumulator(channel6_data, channel6_accumulator)

  for i in range(data_set.size()):
    hardware_timestamp_accumulator.append(data_set.hardware_timestamp[i])
    channel1_accumulator.append(data_set.accelerometer_x[i])
    channel2_accumulator.append(data_set.accelerometer_y[i])
    channel3_accumulator.append(data_set.accelerometer_z[i])
    channel4_accumulator.append(data_set.gyroscope_x[i])
    channel5_accumulator.append(data_set.gyroscope_y[i])
    channel6_accumulator.append(data_set.gyroscope_z[i])

    current_hardware_timestamp = data_set.hardware_timestamp[i]

    if current_hardware_timestamp >= hardware_timestamp_threshold:
      append_accumulators()
      hardware_timestamp_threshold += hardware_timestamp_threshold_offset

  if len(hardware_timestamp_accumulator) != 0:
    append_accumulators()

  for i in range(len(hardware_timestamp_data)):
    df = pd.DataFrame({
        hardware_timestamp_string(): hardware_timestamp_data[i],
        channel1_string(): channel1_data[i],
        channel2_string(): channel2_data[i],
        channel3_string(): channel3_data[i],
        channel4_string(): channel4_data[i],
        channel5_string(): channel5_data[i],
        channel6_string(): channel6_data[i]
    })

    title = f"{csv_file_path}_{filter_kind}_{sensor_to_string(desired_sensor)}_{imu}_{i + 1}".replace(
        " ", "_")
    png_file = f"{title}.png"

    x_size = 11
    y_size = 6
    plt.rc('figure', figsize=(x_size, y_size))
    plot(imu, df)
    plt.legend(loc='upper right')

    plt.title(title)
    plt.ylabel(imu_unit(imu))
    plt.xlabel(f'{hardware_timestamp_string()} (in milliseconds)')
    plt.gca().xaxis.set_major_locator(plt.MultipleLocator(1000))  # A second
    plt.grid()
    plt.savefig(png_file, bbox_inches='tight')
    plt.close()

    print(f"Wrote \"{png_file}\".")


if __name__ == "__main__":
  main()
