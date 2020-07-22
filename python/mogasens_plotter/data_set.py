#!/usr/bin/env python3
# -*- coding: utf8 -*-

import csv

from constants import *


class DataSet:
  def __init__(self, csv_file_path):
    self.time = []
    self.hardware_timestamp = []
    self.extract_id = []
    self.trigger = []
    self.accelerometer_x = []
    self.accelerometer_y = []
    self.accelerometer_z = []
    self.gyroscope_x = []
    self.gyroscope_y = []
    self.gyroscope_z = []

    with open(csv_file_path, 'r') as csv_file:
      plots = csv.reader(csv_file, delimiter=',')
      for row_count, row in enumerate(plots):
        if row_count == 0:  # Skip the header row
          continue

        self.time.append(float(row[time_column_index()]))
        self.hardware_timestamp.append(int(row[hardware_timestamp_index()]))
        self.extract_id.append(int(row[extract_id_column_index()]))
        self.trigger.append(float(row[trigger_index()]))
        self.accelerometer_x.append(float(row[accelerometer_x_column_index()]))
        self.accelerometer_y.append(float(row[accelerometer_y_column_index()]))
        self.accelerometer_z.append(float(row[accelerometer_z_column_index()]))
        self.gyroscope_x.append(float(row[gyroscope_x_column_index()]))
        self.gyroscope_y.append(float(row[gyroscope_y_column_index()]))
        self.gyroscope_z.append(float(row[gyroscope_z_column_index()]))

  def size(self):
    return len(self.time)
