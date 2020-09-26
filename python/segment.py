#!/usr/bin/env python3
# -*- coding: utf8 -*-

import argparse
import sys
import os
import numpy as np
from modules.data_set import DataSet
from modules.segmentation_kind import *
from modules.sensors import sensors
from modules.constants import *
from plotter import main_impl as plotter_main


def validate_sensor(sensor):
  if sensor not in sensors():
    print(f"segment.py: {sensor} is not a valid sensor.", file=sys.stderr)
    return False
  return True


def validate(csv_file_path, sensor, channel, segmentation_kind, window_size):
  if not os.path.isfile(csv_file_path):
    print(f"segment.py: \"{csv_file_path}\" is not a file.", file=sys.stderr)
    return False

  if not validate_sensor(sensor):
    return False

  if channel not in range(1, 7):
    print(
        f"segment.py: {channel} is not a valid channel, must be within 1 - 6.",
        file=sys.stderr)
    return False

  try:
    segmentation_kind_from_str(segmentation_kind)
  except Exception:
    print(
        f"segment.py: \"{segmentation_kind}\" is not a valid segmentation kind.",
        file=sys.stderr)
    return False

  if window_size < 3:
    print(
        f"segment.py: window size was {window_size}, but must be at least 3!",
        file=sys.stderr)
    return False

  if not window_size % 2 == 1:
    print(
        f"segment.py: window size {window_size} is an even number, but an odd number is required.",
        file=sys.stderr)
    return False

  return True


def delete_segmentation_points_if(segmentation_points, condition):
  maximum_index = len(segmentation_points) - 1
  current_index = 1

  while current_index != maximum_index + 1:
    previous_segmentation_point = segmentation_points[current_index - 1]
    current_segmentation_point = segmentation_points[current_index]

    if condition(previous_segmentation_point, current_segmentation_point):
      segmentation_points.pop(current_index)
      maximum_index -= 1
    else:
      current_index += 1


def delete_too_close_segmenting_hardware_timestamps(data_set,
                                                    segmentation_points):
  minimum_distance_milliseconds = 250  # TODO: This may need to change.

  def is_distance_too_small(previous_segmentation_point,
                            current_segmentation_point):
    previous_hardware_timestamp = data_set.hardware_timestamp[
        previous_segmentation_point]
    current_hardware_timestamp = data_set.hardware_timestamp[
        current_segmentation_point]
    distance = current_hardware_timestamp - previous_hardware_timestamp
    return distance < minimum_distance_milliseconds

  delete_segmentation_points_if(segmentation_points, is_distance_too_small)


# TODO: This needs to work on the normed data.
def delete_low_variance_segmentation_points(data_set, segmentation_points,
                                            channel):
  minimum_variance = 0.002  # TODO: This may need to change.

  # TODO: Debug I/O
  print("Total variance: {:f}".format(
      np.var(data_set.channel_by_str(f"channel{channel}"))))

  def is_variance_too_low(previous_segmentation_point,
                          current_segmentation_point):
    desired_channel = data_set.channel_by_str(f"channel{channel}")
    variance = np.var(
        desired_channel[previous_segmentation_point:current_segmentation_point]
    )

    # TODO: Debug I/O
    print("[{};{}): variance is {:f}".format(
        data_set.hardware_timestamp[previous_segmentation_point],
        data_set.hardware_timestamp[current_segmentation_point], variance))

    return variance < minimum_variance

  delete_segmentation_points_if(segmentation_points, is_variance_too_low)


def is_felix_1(csv_file_path):
  return "Felix/2020-07-02_11.17.39" in csv_file_path


def is_felix_2(csv_file_path):
  return "Felix/2020-07-02_12.50.00" in csv_file_path


def is_felix_3(csv_file_path):
  return "Felix/2020-07-02_13.00.09" in csv_file_path


def is_marcelle_1(csv_file_path):
  return "Marcelle/2020-07-02_14.59.59" in csv_file_path


def is_marcelle_2(csv_file_path):
  return "Marcelle/2020-07-02_15.13.22" in csv_file_path


def is_marcelle_3(csv_file_path):
  return "Marcelle/2020-07-02_15.31.36" in csv_file_path


def is_marcelle_4(csv_file_path):
  return "Marcelle/2020-07-02_15.39.22" in csv_file_path


def is_mike_1(csv_file_path):
  return "Mike/2020-07-02_14.07.33" in csv_file_path


def is_mike_2(csv_file_path):
  return "Mike/2020-07-02_14.14.32" in csv_file_path


def is_mike_3(csv_file_path):
  return "Mike/2020-07-02_14.20.28" in csv_file_path


def is_mike_4(csv_file_path):
  return "Mike/2020-07-02_14.38.40" in csv_file_path


def exercise_range(csv_file_path):
  # [begin;end)
  if is_felix_1(csv_file_path):
    return 58227, 108143
  if is_felix_2(csv_file_path):
    return 16003, 63001
  if is_felix_3(csv_file_path):
    return 27003, 62415
  if is_marcelle_1(csv_file_path):
    return 74001, 100001
  if is_marcelle_2(csv_file_path):
    return 23003, 66101
  if is_marcelle_3(csv_file_path):
    return 72271, 113979
  if is_marcelle_4(csv_file_path):
    return 0, 0  # TODO: HERE
  if is_mike_1(csv_file_path):
    return 7003, 44121
  if is_mike_2(csv_file_path):
    return 32003, 65001
  if is_mike_3(csv_file_path):
    return 61995, 84015
  if is_mike_4(csv_file_path):
    return 0, 0  # TODO: HERE
  raise Exception(f'"{csv_file_path}" is not a known CSV file.')


def main(arguments):
  parser = argparse.ArgumentParser(description='Segment a MoGaSens CSV file.')
  parser.add_argument('--image_format',
                      type=str,
                      help='The image format to use e.g. svg',
                      default='png',
                      required=False)
  parser.add_argument('--csv_file_path',
                      type=str,
                      help='Path to the CSV file to segment.',
                      required=True)
  parser.add_argument('--sensor',
                      type=int,
                      help='The sensor to use (769 | 770 | 771 | 772)',
                      required=True)
  parser.add_argument('--channel',  # TODO: This needs to be replaced with IMU (accelerometer / gyroscope)
                      type=int,
                      help='The channel to use (1 | 2 | 3 | 4 | 5 | 6)',
                      required=True)
  parser.add_argument('--segmentation_kind',
                      type=str,
                      help='The segmentation kind to use (min | max | both)',
                      required=True)
  parser.add_argument('--window_size',
                      type=int,
                      help='The window size to use for segmenting.',
                      required=True)
  args = parser.parse_args(arguments)
  csv_file_path = args.csv_file_path
  sensor = args.sensor
  channel = args.channel
  segmentation_kind = args.segmentation_kind
  window_size = args.window_size

  if not validate(csv_file_path, sensor, channel, segmentation_kind,
                  window_size):
    sys.exit(1)

  print(f"\nsegment.py launched with \"{csv_file_path}\".")

  entire_data_set = DataSet.from_file(csv_file_path)
  desired_sensor_data_set = entire_data_set.filter_by_sensor(sensor)
  exercise_begin, exercise_end = exercise_range(csv_file_path)

  if exercise_begin == 0 and exercise_end == 0:
    print(
        f"segment.py: exercise_range returned the empty range for \"{csv_file_path}\", skipping file."
    )
    print(f"segment.py: Exiting.")
    sys.exit(0)

  desired_sensor_data_set.crop_front(exercise_begin)
  desired_sensor_data_set.crop_back(exercise_end)

  segmentation_points = desired_sensor_data_set.segmentation_points(
      f"channel{channel}", segmentation_kind_from_str(segmentation_kind),
      window_size)

  delete_too_close_segmenting_hardware_timestamps(desired_sensor_data_set,
                                                  segmentation_points)
  delete_low_variance_segmentation_points(desired_sensor_data_set,
                                          segmentation_points, channel)

  segmenting_hardware_timestamps = desired_sensor_data_set.segmenting_hardware_timestamps(
      segmentation_points)

  print(
      f"segment.py: {len(segmenting_hardware_timestamps)} segmentation points found in \"{csv_file_path}\"."
  )

  imus = [accelerometer_string(), gyroscope_string()]

  for sensor in sensors():
    for imu in imus:
      plotter_main(
          arguments=[
              '--image_format',
              args.image_format,
              '--no-moving_average_filter',  # Don't use a filter
              '--time_based_split',  # Time based split setting
              csv_file_path,  # Path to the entire CSV file
              f'{sensor}',  # The sensor (left arm, belly, right arm, chest)
              imu,  # The imu (accelerometer / gyroscope)
              '0'  # Filter sample count (must be 0 if none is used)
          ],
          segmenting_hwstamps=segmenting_hardware_timestamps)

  print("segment.py: Done.")
  sys.exit(0)


if __name__ == "__main__":
  main(sys.argv[1:])
