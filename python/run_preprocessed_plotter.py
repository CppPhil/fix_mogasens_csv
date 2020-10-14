#!/usr/bin/env python3
# -*- coding: utf8 -*-

# Sensors
# 769 = left arm
# 770 = belly
# 771 = right arm
# 772 = chest

import argparse
import sys

import modules.preprocessed_plotter

if __name__ == "__main__":
  arguments = sys.argv[1:]
  parser = argparse.ArgumentParser(
      description='Preprocessed plotter CLI parser')
  parser.add_argument('--image_format',
                      type=str,
                      help='The image format to use e.g. svg',
                      default='png',
                      required=False)
  parser.add_argument('csv_file_path',
                      type=str,
                      help='Path to the CSV file to plot.')
  parser.add_argument(
      'imu',
      type=str,
      help='The Inertial Measurement Unit to use (accelerometer / gyroscope).')

  args = parser.parse_args(arguments)

  image_format = args.image_format
  csv_file_path = args.csv_file_path
  imu = args.imu

  modules.preprocessed_plotter.main(image_format=image_format,
                                    is_time_based_split_enabled=True,
                                    csv_file_path=csv_file_path,
                                    imu=imu,
                                    segmenting_hwstamps=[])
  sys.exit(0)
