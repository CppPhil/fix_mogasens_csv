#!/usr/bin/env python3
# -*- coding: utf8 -*-

import csv

from constants import *
from this_sensor import this_sensor


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

    with open(csv_file_name, 'r') as csv_file:
      plots = csv.reader(csv_file, delimiter=',')
      for row_count, row in enumerate(plots):
        if row_count == 0:  # Skip the header row
          continue

        obj.time.append(float(row[time_column_index()]))
        obj.hardware_timestamp.append(int(row[hardware_timestamp_index()]))
        obj.extract_id.append(int(row[extract_id_column_index()]))
        obj.trigger.append(float(row[trigger_index()]))
        obj.accelerometer_x.append(float(row[accelerometer_x_column_index()]))
        obj.accelerometer_y.append(float(row[accelerometer_y_column_index()]))
        obj.accelerometer_z.append(float(row[accelerometer_z_column_index()]))
        obj.gyroscope_x.append(float(row[gyroscope_x_column_index()]))
        obj.gyroscope_y.append(float(row[gyroscope_y_column_index()]))
        obj.gyroscope_z.append(float(row[gyroscope_z_column_index()]))
    return obj

  def size(self):
    return len(self.time)

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
