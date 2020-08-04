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

from modules.constants import *
from modules.data_set import DataSet
from modules.imu_unit import imu_unit
from modules.sensor_to_string import sensor_to_string
from modules.imu_constants import minimum_plotting_value, maximum_plotting_value
from filter import main as filter_main


def plot(the_imu, data_frame, segmenting_hwstamps):
  first_color = 'blue'
  second_color = 'red'
  third_color = 'green'

  first_label = 'x'
  second_label = 'y'
  third_label = 'z'

  line_width = 0.75

  hwstamps = data_frame[hardware_timestamp_string()].to_list()
  lowest = hwstamps[0]
  highest = hwstamps[-1]

  segmentation_points \
      = [hwstamp for hwstamp in segmenting_hwstamps if
         lowest <= hwstamp <= highest] if segmenting_hwstamps is not None else None

  def plot_channel(channel, color, label):
    plt.plot(hardware_timestamp_string(),
             channel,
             data=data_frame,
             color=color,
             label=label,
             linewidth=line_width)

    if segmentation_points is not None:
      for hwstamp in segmentation_points:
        plt.axvline(x=hwstamp, linewidth=line_width, color='magenta')

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
    sys.exit(1)


# noinspection SpellCheckingInspection
def yticks(y_ticks, imu):
  ticks = list(y_ticks[0])
  lowest = ticks[0]
  highest = ticks[-1]

  minimum = minimum_plotting_value(imu)
  maximum = maximum_plotting_value(imu)

  if lowest > minimum:
    ticks.insert(0, minimum)

  if highest < maximum:
    ticks.append(maximum)

  return ticks


def invoke_moving_average_filter(use_moving_average_filter, csv_file_path,
                                 moving_average_filter_sample_count):
  filter_args = []
  if use_moving_average_filter:
    filter_args.append('--moving_average_filter')
  else:
    filter_args.append('--no-moving_average_filter')
  filter_args.append('--csv_file_path')
  filter_args.append(csv_file_path)
  filter_args.append(str(moving_average_filter_sample_count))
  return filter_main(filter_args)


def main(arguments):
  main_impl(arguments, segmenting_hwstamps=None)


def main_impl(arguments, segmenting_hwstamps):
  parser = argparse.ArgumentParser(description='Plot MoGaSens CSV file.')
  parser.add_argument('--image_format',
                      type=str,
                      help='The image format to use e.g. svg',
                      default='png',
                      required=False)
  parser.add_argument('--moving_average_filter',
                      dest='moving_average_filter',
                      action='store_true')
  parser.add_argument('--no-moving_average_filter',
                      dest='moving_average_filter',
                      action='store_false')
  parser.set_defaults(moving_average_filter=False)
  parser.add_argument('--time_based_split',
                      dest='time_based_split',
                      action='store_true')
  parser.add_argument('--no-time_based_split',
                      dest='time_based_split',
                      action='store_false')
  parser.set_defaults(time_based_split=True)
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
  parser.add_argument('moving_average_filter_sample_count',
                      type=int,
                      help='The sample count to use')

  args = parser.parse_args(arguments)

  image_format = args.image_format
  csv_file_path = args.csv_file_path
  desired_sensor = args.sensor
  imu = args.imu
  use_moving_average_filter = args.moving_average_filter
  moving_average_filter_sample_count = args.moving_average_filter_sample_count
  use_time_based_split = args.time_based_split

  if sensor_to_string(desired_sensor).startswith('bogus sensor'):
    print(f"{desired_sensor} is not a valid sensor ID, exiting.",
          file=sys.stderr)
    sys.exit(1)

  filtered_csv_file_path = invoke_moving_average_filter(
      use_moving_average_filter, csv_file_path,
      moving_average_filter_sample_count
  ) if use_moving_average_filter else csv_file_path

  entire_data_set = DataSet.from_file(filtered_csv_file_path)
  data_set = entire_data_set \
      .filter_by_sensor(desired_sensor)

  if data_set.is_empty():
    print(
        f"plotter.py: data set \"{filtered_csv_file_path}\" for sensor {desired_sensor} ({sensor_to_string(desired_sensor)}) was empty, exiting.",
        file=sys.stderr)
    sys.exit(0)

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
      0] + hardware_timestamp_threshold_offset if use_time_based_split else sys.maxsize

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
    try:
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
    except IndexError as err:
      print(
          f"plotter.py: IndexError for file \"{filtered_csv_file_path}\" with index {i}: \"{err}\"",
          file=sys.stderr)
      sys.exit(1)

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

    title = f"{filtered_csv_file_path}_{sensor_to_string(desired_sensor)}_{imu}_{i + 1}".replace(
        " ", "_")
    output_image_file_name = f"{title}.{image_format}"

    x_size = 11
    y_size = 6
    plt.rc('figure', figsize=(x_size, y_size))
    plot(imu, df, segmenting_hwstamps)
    plt.legend(loc='upper right')

    plt.title(title)
    plt.ylabel(imu_unit(imu))
    plt.xlabel(f'{hardware_timestamp_string()} (in milliseconds)')
    plt.gca().xaxis.set_major_locator(plt.MultipleLocator(1000))  # A second

    plt.yticks(yticks(plt.yticks(), imu))

    plt.grid()
    plt.savefig(output_image_file_name,
                bbox_inches='tight',
                format=image_format)
    plt.close()

    print(f"Wrote \"{output_image_file_name}\".")


if __name__ == "__main__":
  main(sys.argv[1:])
