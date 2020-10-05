#!/usr/bin/env python3
# -*- coding: utf8 -*-

import argparse
import sys
import os
import numpy as np
import modules.segmentation_points as segment
from modules.data_set import DataSet
from modules.segmentation_kind import *
from modules.sensors import sensors
from modules.constants import *
from plotter import main_impl as plotter_main
from modules.euclidean_norm import euclidean_norm


def validate_sensor(sensor):
  if sensor not in sensors():
    print(f"segment.py: {sensor} is not a valid sensor.", file=sys.stderr)
    return False
  return True


def validate(csv_file_path, sensor, imu, segmentation_kind, window_size):
  if not os.path.isfile(csv_file_path):
    print(f"segment.py: \"{csv_file_path}\" is not a file.", file=sys.stderr)
    return False

  if not validate_sensor(sensor):
    return False

  if imu not in [accelerometer_string(), gyroscope_string()]:
    print(f"segment.py: {imu} is not a valid IMU.", file=sys.stderr)
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


def delete_low_variance_segmentation_points(normed_data, segmentation_points):
  minimum_variance = 0.002  # TODO: This may need to change.

  def is_variance_too_low(previous_segmentation_point,
                          current_segmentation_point):
    variance = np.var(
        normed_data[previous_segmentation_point:current_segmentation_point])
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


def create_euclidean_norm(data_set, imu):
  if imu == accelerometer_string():
    return euclidean_norm(data_set.accelerometer_x, data_set.accelerometer_y,
                          data_set.accelerometer_z)
  elif imu == gyroscope_string():
    return euclidean_norm(data_set.gyroscope_x, data_set.gyroscope_y,
                          data_set.gyroscope_z)


def main(arguments):
  parser = argparse.ArgumentParser(description='Segment a MoGaSens CSV file.')
  parser.add_argument('--skip_window', dest='skip_window', action='store_true')
  parser.add_argument('--no-skip_window',
                      dest='skip_window',
                      action='store_false')
  parser.add_argument('--delete_too_close',
                      dest='delete_too_close',
                      action='store_true')
  parser.add_argument('--no-delete_too_close',
                      dest='delete_too_close',
                      action='store_false')
  parser.add_argument('--delete_low_variance',
                      dest='delete_low_variance',
                      action='store_true')
  parser.add_argument('--no-delete_low_variance',
                      dest='delete_low_variance',
                      action='store_false')
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
  parser.add_argument('--imu',
                      type=str,
                      help='The IMU to use (accelerometer | gyroscope)',
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
  imu = args.imu
  segmentation_kind = args.segmentation_kind
  window_size = args.window_size
  skip_window = args.skip_window  # Whether to skip the window used for segmentation when a segmentation point is found.
  delete_too_close = args.delete_too_close
  delete_low_variance = args.delete_low_variance

  if not validate(csv_file_path, sensor, imu, segmentation_kind, window_size):
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

  normed_data = create_euclidean_norm(desired_sensor_data_set, imu)

  segmentation_points = segment.segmentation_points(
      normed_data, segmentation_kind_from_str(segmentation_kind), window_size,
      skip_window)

  if delete_too_close:
    delete_too_close_segmenting_hardware_timestamps(desired_sensor_data_set,
                                                    segmentation_points)

  if delete_low_variance:
    delete_low_variance_segmentation_points(normed_data, segmentation_points)

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
