#!/usr/bin/env python3
# -*- coding: utf8 -*-

import argparse
import sys

from modules.data_set import DataSet


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
  parser.add_argument('--out_dir',
                      type=str,
                      help='Path to the directory to write the segments to.',
                      required=True)
  args = parser.parse_args(arguments)
  csv_file_path = args.csv_file_path
  sensor = args.sensor
  channel = args.channel
  out_dir = args.out_dir

  entire_data_set = DataSet.from_file(csv_file_path)
  desired_sensor_data_set = entire_data_set.filter_by_sensor(sensor)

  # TODO: Segment

  # TODO: Write the data sets to csv files.


if __name__ == "__main__":
  main(sys.argv[1:])
