#!/usr/bin/env python3
# -*- coding: utf8 -*-

import argparse
import os
import sys

from modules.data_set import DataSet
from modules.moving_average_filter import moving_average_filter
from modules.sensors import *


def select_filter(use_moving_average_filter):
  if use_moving_average_filter:
    return moving_average_filter
  # If no filter shall be used return a lambda that returns the data set unchanged.
  return lambda sample_count, data: data


def main(arguments):
  # Parse command line arguments
  parser = argparse.ArgumentParser(description='Filter a MoGaSens CSV file.')
  parser.add_argument('--moving_average_filter',
                      dest='moving_average_filter',
                      action='store_true')
  parser.add_argument('--no-moving_average_filter',
                      dest='moving_average_filter',
                      action='store_false')
  parser.set_defaults(moving_average_filter=False)
  parser.add_argument('--csv_file_path',
                      type=str,
                      help='Path to the CSV file to filter.',
                      required=True)
  parser.add_argument('moving_average_filter_sample_count',
                      type=int,
                      help='The sample count to use')
  args = parser.parse_args(arguments)
  csv_file_path = args.csv_file_path
  use_moving_average_filter = args.moving_average_filter
  moving_average_filter_sample_count = args.moving_average_filter_sample_count

  # Detect filter kind
  filter_kind = "no_filter"
  if use_moving_average_filter:
    filter_kind = f"avg_filter_{moving_average_filter_sample_count}"

  # Split the input file path into name and file extension
  file_name, file_extension = os.path.splitext(csv_file_path)

  output_file_name = f"{file_name}_{filter_kind}{file_extension}"

  if os.path.isfile(
      output_file_name):  # Don't do anything if the file's already there.
    return output_file_name

  entire_data_set = DataSet.from_file(
      csv_file_path)  # Load the entire data set
  all_sensors = sensors()  # List of sensor extract IDs
  data_sets = []  # List of the sensor specific data sets

  for sensor in all_sensors:
    # Filter by current sensor
    data_set = entire_data_set.filter_by_sensor(sensor)

    # Apply moving average / rolling mean filter.
    data_set.apply_filter(lambda data: select_filter(use_moving_average_filter)
                          (moving_average_filter_sample_count, data))

    # Add the filtered sensor specific data set to the list
    data_sets.append(data_set)

  # Get rid off data sets that are empty.
  data_sets = list(
      filter(lambda the_data_set: the_data_set.has_elements(), data_sets))

  if len(data_sets) == 0:
    print(f"filter.py: all data sets were empty for file \"{csv_file_path}\"!",
          file=sys.stderr)
    sys.exit(1)

  # Build a combined data set of all the filtered sensor specific data sets
  time = []
  hardware_timestamp = []
  extract_id = []
  trigger = []
  accelerometer_x = []
  accelerometer_y = []
  accelerometer_z = []
  gyroscope_x = []
  gyroscope_y = []
  gyroscope_z = []

  # Create the indices into the sensor specific data sets
  indices = []
  for i in range(len(data_sets)):
    indices.append(0)

  for current_extract_id in entire_data_set.extract_id:
    for i in range(
        len(data_sets)):  # Find the matching sensor specific data set
      current_data_set = data_sets[i]
      current_index = indices[i]

      if current_extract_id == current_data_set.extract_id[0]:
        time.append(current_data_set.time[current_index])
        hardware_timestamp.append(
            current_data_set.hardware_timestamp[current_index])
        extract_id.append(current_data_set.extract_id[current_index])
        trigger.append(current_data_set.trigger[current_index])
        accelerometer_x.append(current_data_set.accelerometer_x[current_index])
        accelerometer_y.append(current_data_set.accelerometer_y[current_index])
        accelerometer_z.append(current_data_set.accelerometer_z[current_index])
        gyroscope_x.append(current_data_set.gyroscope_x[current_index])
        gyroscope_y.append(current_data_set.gyroscope_y[current_index])
        gyroscope_z.append(current_data_set.gyroscope_z[current_index])
        indices[i] += 1  # Increment the index

  # Create the actual data set to be written to a file
  entire_filtered_data_set = DataSet(time, hardware_timestamp, extract_id,
                                     trigger, accelerometer_x, accelerometer_y,
                                     accelerometer_z, gyroscope_x, gyroscope_y,
                                     gyroscope_z)

  # Write the filtered data set of all the sensors to a file
  entire_filtered_data_set.write_to_file(output_file_name)

  return output_file_name


# Entry point
if __name__ == "__main__":
  main(sys.argv[1:])
