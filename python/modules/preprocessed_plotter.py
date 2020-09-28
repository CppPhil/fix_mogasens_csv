#!/usr/bin/env python3
# -*- coding: utf8 -*-

# Sensors
# 769 = left arm
# 770 = belly
# 771 = right arm
# 772 = chest
import sys

import matplotlib.pyplot as plt
import pandas as pd

from modules.constants import hardware_timestamp_string, accelerometer_string, gyroscope_string
from modules.imu_constants import minimum_plotting_value, maximum_plotting_value
from modules.imu_unit import imu_unit
from modules.preprocessed_data_set import PreprocessedDataSet


def plot(the_imu, data_frame, segmenting_hwstamps):
  line_width = 0.75

  hwstamps = data_frame['timestamp_ms'].to_list()
  lowest = hwstamps[0]
  highest = hwstamps[-1]

  segmentation_points \
      = [hwstamp for hwstamp in segmenting_hwstamps if
         lowest <= hwstamp <= highest]

  def plot_channel(channel, color, label):
    plt.plot('timestamp_ms',
             channel,
             data=data_frame,
             color=color,
             label=label,
             linewidth=line_width)

    for hwstamp in segmentation_points:
      plt.axvline(x=hwstamp, linewidth=line_width, color='magenta')

  if the_imu == accelerometer_string():
    # These unfiltered ones are way too noisy to display.
    # plot_channel('x_acc', 'blue', 'x')
    # plot_channel('y_acc', 'red', 'y')
    # plot_channel('z_acc', 'green', 'z')
    # plot_channel('norm_acc', 'orange', 'norm')

    plot_channel('norm_avg_acc', 'aqua', 'norm_avg')
    plot_channel('norm_butter_acc', 'teal', 'norm_butter')

    # TODO: These weird ones seem to be extreme ????
    # plot_channel('rx_acc', 'navy', 'rx')
    # plot_channel('ry_acc', 'olive', 'ry')
    # plot_channel('rz_acc', 'maroon', 'rz')
  elif the_imu == gyroscope_string():
    # Unfiltered data is way too noisy to be displayed.
    # plot_channel('norm_gyro', 'blue', 'norm')

    plot_channel('norm_avg_gyro', 'red', 'norm_avg')
    plot_channel('norm_butter_gyro', 'green', 'norm_butter')
  else:
    print(
        f'preprocessed_plotter.py: imu was "{the_imu}" which is neither "#{accelerometer_string()}" nor "#{gyroscope_string()}", '
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


def main(image_format, is_time_based_split_enabled, csv_file_path, imu,
         segmenting_hwstamps):
  data_set = PreprocessedDataSet.from_file(csv_file_path)

  if data_set.is_empty():
    print(
        f"preprocessed_plotter.py: data set \"{csv_file_path}\" was empty, exiting.",
        file=sys.stderr)
    sys.exit(0)

  timestamp_ms_data = []
  x_acc_data = []
  y_acc_data = []
  z_acc_data = []
  norm_acc_data = []
  norm_avg_acc_data = []
  norm_butter_acc_data = []
  rx_acc_data = []
  ry_acc_data = []
  rz_acc_data = []
  norm_gyro_data = []
  norm_avg_gyro_data = []
  norm_butter_gyro_data = []

  timestamp_ms_accumulator = []
  x_acc_accumulator = []
  y_acc_accumulator = []
  z_acc_accumulator = []
  norm_acc_accumulator = []
  norm_avg_acc_accumulator = []
  norm_butter_acc_accumulator = []
  rx_acc_accumulator = []
  ry_acc_accumulator = []
  rz_acc_accumulator = []
  norm_gyro_accumulator = []
  norm_avg_gyro_accumulator = []
  norm_butter_gyro_accumulator = []

  hardware_timestamp_threshold_offset = 10000  # 10 seconds
  hardware_timestamp_threshold = data_set.timestamp_ms[
      0] + hardware_timestamp_threshold_offset if is_time_based_split_enabled else sys.maxsize

  def append_accumulator(data, accumulator):
    data.append(accumulator.copy())
    accumulator.clear()

  def append_accumulators():
    append_accumulator(timestamp_ms_data, timestamp_ms_accumulator)
    append_accumulator(x_acc_data, x_acc_accumulator)
    append_accumulator(y_acc_data, y_acc_accumulator)
    append_accumulator(z_acc_data, z_acc_accumulator)
    append_accumulator(norm_acc_data, norm_acc_accumulator)
    append_accumulator(norm_avg_acc_data, norm_avg_acc_accumulator)
    append_accumulator(norm_butter_acc_data, norm_butter_acc_accumulator)
    append_accumulator(rx_acc_data, rx_acc_accumulator)
    append_accumulator(ry_acc_data, ry_acc_accumulator)
    append_accumulator(rz_acc_data, rz_acc_accumulator)
    append_accumulator(norm_gyro_data, norm_gyro_accumulator)
    append_accumulator(norm_avg_gyro_data, norm_avg_gyro_accumulator)
    append_accumulator(norm_butter_gyro_data, norm_butter_gyro_accumulator)

  for i in range(data_set.size()):
    try:
      timestamp_ms_accumulator.append(data_set.timestamp_ms[i])
      x_acc_accumulator.append(data_set.x_acc[i])
      y_acc_accumulator.append(data_set.y_acc[i])
      z_acc_accumulator.append(data_set.z_acc[i])
      norm_acc_accumulator.append(data_set.norm_acc[i])
      norm_avg_acc_accumulator.append(data_set.norm_avg_acc[i])
      norm_butter_acc_accumulator.append(data_set.norm_butter_acc[i])
      rx_acc_accumulator.append(data_set.rx_acc[i])
      ry_acc_accumulator.append(data_set.ry_acc[i])
      rz_acc_accumulator.append(data_set.rz_acc[i])
      norm_gyro_accumulator.append(data_set.norm_gyro[i])
      norm_avg_gyro_accumulator.append(data_set.norm_avg_gyro[i])
      norm_butter_gyro_accumulator.append(data_set.norm_butter_gyro[i])

      current_hardware_timestamp = data_set.timestamp_ms[i]

      if current_hardware_timestamp >= hardware_timestamp_threshold:
        append_accumulators()
        hardware_timestamp_threshold += hardware_timestamp_threshold_offset
    except IndexError as err:
      print(
          f"preprocessed_plotter.py: IndexError for file \"{csv_file_path}\" with index {i}: \"{err}\"",
          file=sys.stderr)
      sys.exit(1)

  if len(timestamp_ms_accumulator) != 0:
    append_accumulators()

  for i in range(len(timestamp_ms_data)):
    df = pd.DataFrame({
        'timestamp_ms': timestamp_ms_data[i],
        'x_acc': x_acc_data[i],
        'y_acc': y_acc_data[i],
        'z_acc': z_acc_data[i],
        'norm_acc': norm_acc_data[i],
        'norm_avg_acc': norm_avg_acc_data[i],
        'norm_butter_acc': norm_butter_acc_data[i],
        'rx_acc': rx_acc_data[i],
        'ry_acc': ry_acc_data[i],
        'rz_acc': rz_acc_data[i],
        'norm_gyro': norm_gyro_data[i],
        'norm_avg_gyro': norm_avg_gyro_data[i],
        'norm_butter_gyro': norm_butter_gyro_data[i]
    })

    title = f"{csv_file_path}_{imu}_{i + 1}".replace(" ", "_")
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

    print(f"preprocessed_plotter.py: Wrote \"{output_image_file_name}\".")
