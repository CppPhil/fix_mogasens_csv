#!/usr/bin/env python3
# -*- coding: utf8 -*-
import argparse
import os
import sys

import numpy as np

import modules.segmentation_points as segment
import modules.preprocessed_plotter

from modules.constants import accelerometer_string, gyroscope_string
from modules.preprocessed_data_set import PreprocessedDataSet
from modules.segmentation_kind import segmentation_kind_from_str
from modules.export_with_repetition_ids import export_with_repetition_ids


def validate(csv_file_path, imu, segmentation_kind, window_size, filter_data):
  if not os.path.isfile(csv_file_path):
    print(f"preprocessed_segment.py: \"{csv_file_path}\" is not a file.",
          file=sys.stderr)
    return False

  if imu not in [accelerometer_string(), gyroscope_string()]:
    print(f"preprocessed_segment.py: {imu} is not a valid IMU.",
          file=sys.stderr)
    return False

  try:
    segmentation_kind_from_str(segmentation_kind)
  except Exception:
    print(
        f"preprocessed_segment.py: \"{segmentation_kind}\" is not a valid segmentation kind.",
        file=sys.stderr)
    return False

  if window_size < 3:
    print(
        f"preprocessed_segment.py: window size was {window_size}, but must be at least 3!",
        file=sys.stderr)
    return False

  if not window_size % 2 == 1:
    print(
        f"preprocessed_segment.py: window size {window_size} is an even number, but an odd number is required.",
        file=sys.stderr)
    return False

  if (filter_data != 'average') and (filter_data != 'butterworth'):
    print(
        f"preprocessed_segment.py: filter {filter_data} must be average or butterworth!",
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
    previous_hardware_timestamp = data_set.timestamp_ms[
        previous_segmentation_point]
    current_hardware_timestamp = data_set.timestamp_ms[
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
  return "2020-07-02_11.17.39" in csv_file_path


def is_felix_2(csv_file_path):
  return "2020-07-02_12.50.00" in csv_file_path


def is_felix_3(csv_file_path):
  return "2020-07-02_13.00.09" in csv_file_path


def is_marcelle_1(csv_file_path):
  return "2020-07-02_14.59.59" in csv_file_path


def is_marcelle_2(csv_file_path):
  return "2020-07-02_15.13.22" in csv_file_path


def is_marcelle_3(csv_file_path):
  return "2020-07-02_15.31.36" in csv_file_path


def is_marcelle_4(csv_file_path):
  return "2020-07-02_15.39.22" in csv_file_path


def is_mike_1(csv_file_path):
  return "2020-07-02_14.07.33" in csv_file_path


def is_mike_2(csv_file_path):
  return "2020-07-02_14.14.32" in csv_file_path


def is_mike_3(csv_file_path):
  return "2020-07-02_14.20.28" in csv_file_path


def is_mike_4(csv_file_path):
  return "2020-07-02_14.38.40" in csv_file_path


def is_jan_push_up_1(csv_file_path):
  return "Jan_liegestuetzen1" in csv_file_path


def is_jan_push_up_2(csv_file_path):
  return "Jan_liegestuetzen2" in csv_file_path


def is_jan_push_up_3(csv_file_path):
  return "Jan_liegestuetzen3" in csv_file_path


def is_jan_plank_4(csv_file_path):
  return "Jan_Plank4" in csv_file_path


def is_andre_idle_7(csv_file_path):
  return "Andre_Idle7" in csv_file_path


def is_andre_push_up_1(csv_file_path):
  return "Andre_liegestuetzen1" in csv_file_path


def is_andre_push_up_2(csv_file_path):
  return "Andre_liegestuetzen2" in csv_file_path


def is_andre_push_up_3(csv_file_path):
  return "Andre_liegestuetzen3" in csv_file_path


def is_andre_plank_4(csv_file_path):
  return "Andre_Plank4" in csv_file_path


def is_andre_squats_1(csv_file_path):
  return "Andre_Squats" in csv_file_path and not is_andre_squats_2(
      csv_file_path)


def is_andre_squats_2(csv_file_path):
  return "Andre_Squats2" in csv_file_path


def is_lucas_push_up_1(csv_file_path):
  return "Lukas_liegestuetzen1" in csv_file_path


def is_lucas_push_up_2(csv_file_path):
  return "Lukas_liegestuetzen2" in csv_file_path


def is_lucas_push_up_3(csv_file_path):
  return "Lukas_liegestuetzen3" in csv_file_path


def is_lucas_plank_4(csv_file_path):
  return "Lukas_Plank4" in csv_file_path


def exercise_range(csv_file_path):
  # [begin;end)
  if is_felix_1(csv_file_path):
    return 58227, 108143
  if is_felix_2(csv_file_path):
    return 16003, 62999
  if is_felix_3(csv_file_path):
    return 26999, 62415
  if is_marcelle_1(csv_file_path):
    return 73999, 99999
  if is_marcelle_2(csv_file_path):
    return 23003, 66099
  if is_marcelle_3(csv_file_path):
    return 72271, 113979
  if is_marcelle_4(csv_file_path):
    return 0, 0  # TODO: HERE
  if is_mike_1(csv_file_path):
    return 7005, 44121
  if is_mike_2(csv_file_path):
    return 32003, 64999
  if is_mike_3(csv_file_path):
    return 61995, 84011
  if is_mike_4(csv_file_path):
    return 0, 0  # TODO: HERE
  if is_jan_push_up_1(csv_file_path):
    return 84900, 184000
  if is_jan_push_up_2(csv_file_path):
    return 16996, 51001
  if is_jan_push_up_3(csv_file_path):
    return 34000, 57000
  if is_jan_plank_4(csv_file_path):
    return 0, 0  # TODO: HERE
  if is_andre_idle_7(csv_file_path):
    return 0, 0  # TODO: HERE
  if is_andre_push_up_1(csv_file_path):
    return 16996, 67001
  if is_andre_push_up_2(csv_file_path):
    return 39996, 80001
  if is_andre_push_up_3(csv_file_path):
    return 30998, 65503
  if is_andre_plank_4(csv_file_path):
    return 0, 0  # TODO: HERE
  if is_andre_squats_1(csv_file_path):
    return 60996, 159001
  if is_andre_squats_2(csv_file_path):
    return 47500, 214000
  if is_lucas_push_up_1(csv_file_path):
    return 38000, 84000
  if is_lucas_push_up_2(csv_file_path):
    return 36000, 74000
  if is_lucas_push_up_3(csv_file_path):
    return 10996, 28801
  if is_lucas_plank_4(csv_file_path):
    return 0, 0  # TODO: HERE
  raise Exception(f'"{csv_file_path}" is not a known CSV file.')


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
  parser.add_argument('--filter',
                      type=str,
                      help='Which data to use (average | butterworth)',
                      required=True)
  args = parser.parse_args(arguments)
  csv_file_path = args.csv_file_path
  imu = args.imu
  segmentation_kind = args.segmentation_kind
  window_size = args.window_size
  skip_window = args.skip_window  # Whether to skip the window used for segmentation when a segmentation point is found.
  delete_too_close = args.delete_too_close
  delete_low_variance = args.delete_low_variance
  filter_data = args.filter

  if not validate(csv_file_path, imu, segmentation_kind, window_size,
                  filter_data):
    sys.exit(1)

  data_set = PreprocessedDataSet.from_file(csv_file_path)
  exercise_begin, exercise_end = exercise_range(csv_file_path)

  if exercise_begin == 0 and exercise_end == 0:
    # TODO: HERE This is commented out temporarily.
    # print(
    #    f"preprocessed_segment.py: exercise_range returned the empty range for \"{csv_file_path}\", skipping file."
    # )
    # print(f"preprocessed_segment.py: Exiting.")
    sys.exit(0)

  data_set.crop_front(exercise_begin)
  data_set.crop_back(exercise_end)

  if filter_data == 'butterworth':  # Butterworth filter
    normed_data = data_set.norm_butter_acc if imu == accelerometer_string(
    ) else data_set.norm_butter_gyro
  else:  # Moving average filter
    normed_data = data_set.norm_avg_acc if imu == accelerometer_string(
    ) else data_set.norm_avg_gyro

  segmentation_points = segment.segmentation_points(
      normed_data, segmentation_kind_from_str(segmentation_kind), window_size,
      skip_window)

  if delete_too_close:
    delete_too_close_segmenting_hardware_timestamps(data_set,
                                                    segmentation_points)

  if delete_low_variance:
    delete_low_variance_segmentation_points(normed_data, segmentation_points)

  segmenting_hardware_timestamps = data_set.segmenting_hardware_timestamps(
      segmentation_points)

  print(
      f"preprocessed_segment.py: {len(segmenting_hardware_timestamps)} segmentation points found in \"{csv_file_path}\"."
  )

  print(
      f"preprocessed_segment.py: \"{csv_file_path}\" contains the following segmentation points:\n{segmenting_hardware_timestamps}",
      file=sys.stderr)

  file_name, file_extension = os.path.splitext(csv_file_path)

  export_with_repetition_ids(
      data_set, segmenting_hardware_timestamps,
      f"{file_name}_with_repetition_ids.{file_extension}")

  # TODO: HERE This stuff is commented out temporarily.
  #  imus = [accelerometer_string(), gyroscope_string()]

  #  for imu in imus:
  #    modules.preprocessed_plotter.main(
  #        image_format=args.image_format,
  #        is_time_based_split_enabled=True,
  #        csv_file_path=csv_file_path,
  #        imu=imu,
  #        segmenting_hwstamps=segmenting_hardware_timestamps)

  sys.exit(0)


if __name__ == "__main__":
  main(sys.argv[1:])
