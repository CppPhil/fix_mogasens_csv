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
  parser.add_argument('--out_dir',
                      type=str,
                      help='Path to the directory to write the segments to.',
                      required=True)
  args = parser.parse_args(arguments)
  csv_file_path = args.csv_file_path
  out_dir = args.out_dir

  # TODO: Put in the sensor and the channel by which to segment everything.

  entire_data_set = DataSet.from_file(csv_file_path)

  # TODO: Filter by sensor.

  # TODO: Segment

  # TODO: Write the data sets to csv files.


if __name__ == "__main__":
  main(sys.argv[1:])
