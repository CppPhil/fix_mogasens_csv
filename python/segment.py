#!/usr/bin/env python3
# -*- coding: utf8 -*-

import argparse
import sys
import os

from modules.data_set import DataSet
from modules.segmentation_kind import *
from modules.sensors import sensors


def validate_sensor(sensor):
  if sensor not in sensors():
    print(f"{sensor} is not a valid sensor.", file=sys.stderr)
    return False
  return True


def validate(csv_file_path, sensor, channel, segmentation_kind, out_dir):
  if not os.path.isfile(csv_file_path):
    print(f"\"{csv_file_path}\" is not a file.", file=sys.stderr)
    return False

  if not validate_sensor(sensor):
    return False

  if channel not in range(1, 7):
    print(f"{channel} is not a valid channel, must be within 1 - 6.",
          file=sys.stderr)
    return False

  try:
    segmentation_kind_from_str(segmentation_kind)
  except Exception:
    print(f"\"{segmentation_kind}\" is not a valid segmentation kind.",
          file=sys.stderr)
    return False

  if not os.path.isdir(out_dir):
    print(f"\"{out_dir}\" is not a directory.", file=sys.stderr)
    return False

  return True


def main(arguments):
  parser = argparse.ArgumentParser(description='Segment a MoGaSens CSV file.')
  parser.add_argument('--csv_file_path',
                      type=str,
                      help='Path to the CSV file to segment.',
                      required=True)
  parser.add_argument('--sensor',
                      type=int,
                      help='The sensor to use (769 | 770 | 771 | 772)',
                      required=True)
  parser.add_argument('--channel',
                      type=int,
                      help='The channel to use (1 | 2 | 3 | 4 | 5 | 6)',
                      required=True)
  parser.add_argument('--segmentation_kind',
                      type=str,
                      help='The segmentation kind to use (min | max | both)',
                      required=True)
  parser.add_argument('--out_dir',
                      type=str,
                      help='Path to the directory to write the segments to.',
                      required=True)
  args = parser.parse_args(arguments)
  csv_file_path = args.csv_file_path
  sensor = args.sensor
  channel = args.channel
  segmentation_kind = args.segmentation_kind
  out_dir = args.out_dir

  if not validate(csv_file_path, sensor, channel, segmentation_kind, out_dir):
    sys.exit(1)

  entire_data_set = DataSet.from_file(csv_file_path)
  desired_sensor_data_set = entire_data_set.filter_by_sensor(sensor)
  segmenting_hardware_timestamps = desired_sensor_data_set.segmenting_hardware_timestamps(
      f"channel{channel}", segmentation_kind_from_str(segmentation_kind))
  segments = entire_data_set.segment_by(segmenting_hardware_timestamps)

  base_name = os.path.basename(csv_file_path)
  file_name, extension = os.path.splitext(base_name)

  for i in range(len(segments)):
    current_segment = segments[i]
    current_segment.write_to_file(
        f"{out_dir}/{file_name}_segment_{i}{extension}")

  exit(0)


if __name__ == "__main__":
  main(sys.argv[1:])
