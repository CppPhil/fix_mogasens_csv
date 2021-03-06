#!/usr/bin/env python3
# -*- coding: utf8 -*-
import _csv
import csv
import sys
import traceback

from .constants import *
from .this_sensor import this_sensor


class DataSet:
  def __init__(self, time, hardware_timestamp, extract_id, trigger,
               accelerometer_x, accelerometer_y, accelerometer_z, gyroscope_x,
               gyroscope_y, gyroscope_z):
    self.time = time
    self.hardware_timestamp = hardware_timestamp
    self.extract_id = extract_id
    self.trigger = trigger
    self.accelerometer_x = accelerometer_x
    self.accelerometer_y = accelerometer_y
    self.accelerometer_z = accelerometer_z
    self.gyroscope_x = gyroscope_x
    self.gyroscope_y = gyroscope_y
    self.gyroscope_z = gyroscope_z

  @classmethod
  def from_file(cls, csv_file_name):
    obj = cls.__new__(cls)
    super(DataSet, obj).__init__()
    obj.time = []
    obj.hardware_timestamp = []
    obj.extract_id = []
    obj.trigger = []
    obj.accelerometer_x = []
    obj.accelerometer_y = []
    obj.accelerometer_z = []
    obj.gyroscope_x = []
    obj.gyroscope_y = []
    obj.gyroscope_z = []

    with open(csv_file_name, 'r', newline='', encoding='utf-8') as csv_file:
      try:
        plots = csv.reader(csv_file, delimiter=',')
        for row_count, row in enumerate(plots):
          if row_count == 0:  # Skip the header row
            continue

          try:
            obj.time.append(float(row[time_column_index()]))
            obj.hardware_timestamp.append(int(row[hardware_timestamp_index()]))
            obj.extract_id.append(int(row[extract_id_column_index()]))
            obj.trigger.append(float(row[trigger_index()]))
            obj.accelerometer_x.append(
                float(row[accelerometer_x_column_index()]))
            obj.accelerometer_y.append(
                float(row[accelerometer_y_column_index()]))
            obj.accelerometer_z.append(
                float(row[accelerometer_z_column_index()]))
            obj.gyroscope_x.append(float(row[gyroscope_x_column_index()]))
            obj.gyroscope_y.append(float(row[gyroscope_y_column_index()]))
            obj.gyroscope_z.append(float(row[gyroscope_z_column_index()]))
          except IndexError as err:
            print(
                f"data_set.py: DataSet.from_file: IndexError for file \"{csv_file_name}\": \"{err}\", row_count: {row_count}, row: {row}",
                file=sys.stderr)
            traceback.print_exc(file=sys.stderr)
            sys.exit(1)
          except ValueError as value_error:
            print(
                f"data_set.py: DataSet.from_file: ValueError for file \"{csv_file_name}\": \"{value_error}\", row_count: {row_count}, row: {row}",
                file=sys.stderr)
            traceback.print_exc(file=sys.stderr)
            sys.exit(1)
      except _csv.Error as err:
        print(
            f"data_set.py: DataSet.from_file: _csv.Error for file \"{csv_file_name}\": \"{err}\"",
            file=sys.stderr)
        sys.exit(1)
    return obj

  def size(self):
    return len(self.time)

  def is_empty(self):
    return self.size() == 0

  def has_elements(self):
    return not self.is_empty()

  def filter_by_sensor(self, sensor):
    this_sensor_time, this_sensor_hardware_timestamp, this_sensor_extract_id, this_sensor_trigger, this_sensor_accelerometer_x, this_sensor_accelerometer_y, this_sensor_accelerometer_z, this_sensor_gyroscope_x, this_sensor_gyroscope_y, this_sensor_gyroscope_z = this_sensor(
        sensor, self.time, self.hardware_timestamp, self.extract_id,
        self.trigger, self.accelerometer_x, self.accelerometer_y,
        self.accelerometer_z, self.gyroscope_x, self.gyroscope_y,
        self.gyroscope_z)
    return DataSet(this_sensor_time, this_sensor_hardware_timestamp,
                   this_sensor_extract_id, this_sensor_trigger,
                   this_sensor_accelerometer_x, this_sensor_accelerometer_y,
                   this_sensor_accelerometer_z, this_sensor_gyroscope_x,
                   this_sensor_gyroscope_y, this_sensor_gyroscope_z)

  def apply_filter(self, func):
    self.accelerometer_x = func(self.accelerometer_x)
    self.accelerometer_y = func(self.accelerometer_y)
    self.accelerometer_z = func(self.accelerometer_z)
    self.gyroscope_x = func(self.gyroscope_x)
    self.gyroscope_y = func(self.gyroscope_y)
    self.gyroscope_z = func(self.gyroscope_z)
    return self

  def write_to_file(self, file_path):
    with open(file_path, 'w', newline='\n', encoding='utf-8') as csv_file:
      writer = csv.writer(csv_file,
                          delimiter=',',
                          quotechar='"',
                          quoting=csv.QUOTE_MINIMAL)
      writer.writerow([
          'Time (s)', 'HWTimestamp (ms)', 'ExtractID', 'Trigger', 'Channel 1',
          'Channel 2', 'Channel 3', 'Channel 4', 'Channel 5', 'Channel 6'
      ])

      for i in range(self.size()):
        writer.writerow([
            self.time[i], self.hardware_timestamp[i], self.extract_id[i],
            self.trigger[i], self.accelerometer_x[i], self.accelerometer_y[i],
            self.accelerometer_z[i], self.gyroscope_x[i], self.gyroscope_y[i],
            self.gyroscope_z[i]
        ])

  def channel_by_str(self, string):
    if string == channel1_string():
      return self.accelerometer_x

    if string == channel2_string():
      return self.accelerometer_y

    if string == channel3_string():
      return self.accelerometer_z

    if string == channel4_string():
      return self.gyroscope_x

    if string == channel5_string():
      return self.gyroscope_y

    if string == channel6_string():
      return self.gyroscope_z

    raise Exception(f"\"{string}\" is not a valid input to channel_by_str!")

  def segmenting_hardware_timestamps(self, segmentation_points):
    return [
        self.hardware_timestamp[segmentation_point]
        for segmentation_point in segmentation_points
    ]

  def segment_by(self, segmenting_hwstamps):
    segments = []
    last_idx = 0

    for hwstamp in segmenting_hwstamps:
      idx = self.hardware_timestamp.index(hwstamp)
      segments.append(
          DataSet(self.time[last_idx:idx],
                  self.hardware_timestamp[last_idx:idx],
                  self.extract_id[last_idx:idx], self.trigger[last_idx:idx],
                  self.accelerometer_x[last_idx:idx],
                  self.accelerometer_y[last_idx:idx],
                  self.accelerometer_z[last_idx:idx],
                  self.gyroscope_x[last_idx:idx],
                  self.gyroscope_y[last_idx:idx],
                  self.gyroscope_z[last_idx:idx]))
      last_idx = idx

    segments.append(
        DataSet(self.time[last_idx:], self.hardware_timestamp[last_idx:],
                self.extract_id[last_idx:], self.trigger[last_idx:],
                self.accelerometer_x[last_idx:],
                self.accelerometer_y[last_idx:],
                self.accelerometer_z[last_idx:], self.gyroscope_x[last_idx:],
                self.gyroscope_y[last_idx:], self.gyroscope_z[last_idx:]))

    return segments

  def crop_front(self, exercise_start_timestamp):
    # inclusive
    crop_index = self.hardware_timestamp.index(exercise_start_timestamp)
    self.time = self.time[crop_index:]
    self.hardware_timestamp = self.hardware_timestamp[crop_index:]
    self.extract_id = self.extract_id[crop_index:]
    self.trigger = self.trigger[crop_index:]
    self.accelerometer_x = self.accelerometer_x[crop_index:]
    self.accelerometer_y = self.accelerometer_y[crop_index:]
    self.accelerometer_z = self.accelerometer_z[crop_index:]
    self.gyroscope_x = self.gyroscope_x[crop_index:]
    self.gyroscope_y = self.gyroscope_y[crop_index:]
    self.gyroscope_z = self.gyroscope_z[crop_index:]

  def crop_back(self, exercise_end_timestamp):
    # exclusive
    crop_index = self.hardware_timestamp.index(exercise_end_timestamp)
    self.time = self.time[:crop_index]
    self.hardware_timestamp = self.hardware_timestamp[:crop_index]
    self.extract_id = self.extract_id[:crop_index]
    self.trigger = self.trigger[:crop_index]
    self.accelerometer_x = self.accelerometer_x[:crop_index]
    self.accelerometer_y = self.accelerometer_y[:crop_index]
    self.accelerometer_z = self.accelerometer_z[:crop_index]
    self.gyroscope_x = self.gyroscope_x[:crop_index]
    self.gyroscope_y = self.gyroscope_y[:crop_index]
    self.gyroscope_z = self.gyroscope_z[:crop_index]
