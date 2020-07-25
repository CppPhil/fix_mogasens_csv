#!/usr/bin/env python3
# -*- coding: utf8 -*-

import argparse
import os

from modules.data_set import DataSet
from modules.sensors import sensors
from modules.moving_average_filter import moving_average_filter


def select_filter(use_moving_average_filter):
  if use_moving_average_filter:
    return moving_average_filter
  return lambda sample_count, data: data  # If no filter shall be used return a lambda that returns the data set unchanged.


def main():
  # Parse command line arguments
  parser = argparse.ArgumentParser(description='Filter a MoGaSens CSV file.')
  parser.add_argument('csv_file_path',
                      type=str,
                      help='Path to the CSV file to filter.')
  parser.add_argument('--moving_average_filter',
                      dest='moving_average_filter',
                      action='store_true')
  parser.add_argument('--no-moving_average_filter',
                      dest='moving_average_filter',
                      action='store_false')
  parser.set_defaults(moving_average_filter=False)
  parser.add_argument('moving_average_filter_sample_count',
                      type=int,
                      help='The sample count to use')
  args = parser.parse_args()
  csv_file_path = args.csv_file_path
  use_moving_average_filter = args.moving_average_filter
  moving_average_filter_sample_count = args.moving_average_filter_sample_count

  # Detect filter kind
  filter_kind = "no_filter"
  if use_moving_average_filter:
    filter_kind = f"avg_filter_{moving_average_filter_sample_count}"

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

  for i in range(len(data_sets[0].time)):
    for data_set in data_sets:
      time.append(data_set.time[i])
      hardware_timestamp.append(data_set.hardware_timestamp[i])
      extract_id.append(data_set.extract_id[i])
      trigger.append(data_set.trigger[i])
      accelerometer_x.append(data_set.accelerometer_x[i])
      accelerometer_y.append(data_set.accelerometer_y[i])
      accelerometer_z.append(data_set.accelerometer_z[i])
      gyroscope_x.append(data_set.gyroscope_x[i])
      gyroscope_y.append(data_set.gyroscope_y[i])
      gyroscope_z.append(data_set.gyroscope_z[i])

  # Create the actual data set to be written to a file
  entire_filtered_data_set = DataSet(time, hardware_timestamp, extract_id,
                                     trigger, accelerometer_x, accelerometer_y,
                                     accelerometer_z, gyroscope_x, gyroscope_y,
                                     gyroscope_z)

  # Split the input file path into name and file extension
  file_name, file_extension = os.path.splitext(csv_file_path)

  # Write the filtered data set of all the sensors to a file
  entire_filtered_data_set.write_to_file(
      f"{file_name}_{filter_kind}{file_extension}")


# Entry point
if __name__ == "__main__":
  main()
