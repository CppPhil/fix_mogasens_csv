#!/usr/bin/env python3
# -*- coding: utf8 -*-
import _csv
import csv
import sys
import traceback


class PreprocessedDataSet:
  def __init__(self, time_s, timestamp_ms, x_acc, y_acc, z_acc, x_acc_avg,
               y_acc_avg, z_acc_avg, x_acc_butter, y_acc_butter, z_acc_butter,
               norm_acc, norm_avg_acc, norm_butter_acc, rx_acc, ry_acc, rz_acc,
               rx_acc_avg, ry_acc_avg, rz_acc_avg, rx_acc_butter,
               ry_acc_butter, rz_acc_butter, norm_gyro, norm_avg_gyro,
               norm_butter_gyro):
    self.time_s = time_s
    self.timestamp_ms = timestamp_ms
    self.x_acc = x_acc
    self.y_acc = y_acc
    self.z_acc = z_acc
    self.x_acc_avg = x_acc_avg
    self.y_acc_avg = y_acc_avg
    self.z_acc_avg = z_acc_avg
    self.x_acc_butter = x_acc_butter
    self.y_acc_butter = y_acc_butter
    self.z_acc_butter = z_acc_butter
    self.norm_acc = norm_acc
    self.norm_avg_acc = norm_avg_acc
    self.norm_butter_acc = norm_butter_acc
    self.rx_acc = rx_acc
    self.ry_acc = ry_acc
    self.rz_acc = rz_acc
    self.rx_acc_avg = rx_acc_avg
    self.ry_acc_avg = ry_acc_avg
    self.rz_acc_avg = rz_acc_avg
    self.rx_acc_butter = rx_acc_butter
    self.ry_acc_butter = ry_acc_butter
    self.rz_acc_butter = rz_acc_butter
    self.norm_gyro = norm_gyro
    self.norm_avg_gyro = norm_avg_gyro
    self.norm_butter_gyro = norm_butter_gyro

  @classmethod
  def from_file(cls, csv_file_name):
    obj = cls.__new__(cls)
    super(PreprocessedDataSet, obj).__init__()
    obj.time_s = []
    obj.timestamp_ms = []
    obj.x_acc = []
    obj.y_acc = []
    obj.z_acc = []
    obj.x_acc_avg = []
    obj.y_acc_avg = []
    obj.z_acc_avg = []
    obj.x_acc_butter = []
    obj.y_acc_butter = []
    obj.z_acc_butter = []
    obj.norm_acc = []
    obj.norm_avg_acc = []
    obj.norm_butter_acc = []
    obj.rx_acc = []
    obj.ry_acc = []
    obj.rz_acc = []
    obj.rx_acc_avg = []
    obj.ry_acc_avg = []
    obj.rz_acc_avg = []
    obj.rx_acc_butter = []
    obj.ry_acc_butter = []
    obj.rz_acc_butter = []
    obj.norm_gyro = []
    obj.norm_avg_gyro = []
    obj.norm_butter_gyro = []

    with open(csv_file_name, 'r', newline='', encoding='utf-8') as csv_file:
      try:
        plots = csv.reader(csv_file, delimiter=',')
        for row_count, row in enumerate(plots):
          if row_count == 0:  # Skip the header row
            continue

          try:
            obj.time_s.append(float(row[0]))
            obj.timestamp_ms.append(int(row[1]))
            obj.x_acc.append(float(row[2]))
            obj.y_acc.append(float(row[3]))
            obj.z_acc.append(float(row[4]))
            obj.x_acc_avg.append(float(row[5]))
            obj.y_acc_avg.append(float(row[6]))
            obj.z_acc_avg.append(float(row[7]))
            obj.x_acc_butter.append(float(row[8]))
            obj.y_acc_butter.append(float(row[9]))
            obj.z_acc_butter.append(float(row[10]))
            obj.norm_acc.append(float(row[11]))
            obj.norm_avg_acc.append(float(row[12]))
            obj.norm_butter_acc.append(float(row[13]))
            obj.rx_acc.append(float(row[14]))
            obj.ry_acc.append(float(row[15]))
            obj.rz_acc.append(float(row[16]))
            obj.rx_acc_avg.append(float(row[17]))
            obj.ry_acc_avg.append(float(row[18]))
            obj.rz_acc_avg.append(float(row[19]))
            obj.rx_acc_butter.append(float(row[20]))
            obj.ry_acc_butter.append(float(row[21]))
            obj.rz_acc_butter.append(float(row[22]))
            obj.norm_gyro.append(float(row[23]))
            obj.norm_avg_gyro.append(float(row[24]))
            obj.norm_butter_gyro.append(float(row[25]))
          except IndexError as err:
            print(
                f"preprocessed_data_set.py: PreprocessedDataSet.from_file: IndexError for file \"{csv_file_name}\": \"{err}\", row_count: {row_count}, row: {row}",
                file=sys.stderr)
            traceback.print_exc(file=sys.stderr)
            sys.exit(1)
          except ValueError as value_error:
            print(
                f"preprocessed_data_set.py: PreprocessedDataSet.from_file: ValueError for file \"{csv_file_name}\": \"{value_error}\", row_count: {row_count}, row: {row}",
                file=sys.stderr)
            traceback.print_exc(file=sys.stderr)
            sys.exit(1)
      except _csv.Error as err:
        print(
            f"preprocessed_data_set.py: PreprocessedDataSet.from_file: _csv.Error for file \"{csv_file_name}\": \"{err}\"",
            file=sys.stderr)
        sys.exit(1)
    return obj

  def size(self):
    return len(self.time_s)

  def is_empty(self):
    return self.size() == 0

  def has_elements(self):
    return not self.is_empty()

  def write_to_file(self, file_path):
    with open(file_path, 'w', newline='\n', encoding='utf-8') as csv_file:
      writer = csv.writer(csv_file,
                          delimiter=',',
                          quotechar='"',
                          quoting=csv.QUOTE_MINIMAL)
      writer.writerow([
          'time_s', 'timestamp_ms', 'xAcc', 'yAcc', 'zAcc', 'xAccAvg',
          'yAccAvg', 'zAccAvg', 'xAccButter', 'yAccButter', 'zAccButter',
          'normAcc', 'normAvgAcc', 'normButterAcc', 'rxAcc', 'ryAcc', 'rzAcc',
          'rxAccAvg', 'ryAccAvg', 'rzAccAvg', 'rxAccButter', 'ryAccButter',
          'rzAccButter', 'normGyro', 'normAvgGyro', 'normButterGyro'
      ])

      for i in range(self.size()):
        writer.writerow([
            self.time_s[i], self.timestamp_ms[i], self.x_acc[i], self.y_acc[i],
            self.z_acc[i], self.x_acc_avg[i], self.y_acc_avg[i],
            self.z_acc_avg[i], self.x_acc_butter[i], self.y_acc_butter[i],
            self.z_acc_butter[i], self.norm_acc[i], self.norm_avg_acc[i],
            self.norm_butter_acc[i], self.rx_acc[i], self.ry_acc[i],
            self.rz_acc[i], self.rx_acc_avg[i], self.ry_acc_avg[i],
            self.rz_acc_avg[i], self.rx_acc_butter[i], self.ry_acc_butter[i],
            self.rz_acc_butter[i], self.norm_gyro[i], self.norm_avg_gyro[i],
            self.norm_butter_gyro[i]
        ])

  def segmenting_hardware_timestamps(self, segmentation_points):
    return [
        self.timestamp_ms[segmentation_point]
        for segmentation_point in segmentation_points
    ]

  def segment_by(self, segmenting_hwstamps):
    segments = []
    last_idx = 0

    for hwstamp in segmenting_hwstamps:
      idx = self.timestamp_ms.index(hwstamp)
      segments.append(
          PreprocessedDataSet(
              self.time_s[last_idx:idx], self.timestamp_ms[last_idx:idx],
              self.x_acc[last_idx:idx], self.y_acc[last_idx:idx],
              self.z_acc[last_idx:idx], self.x_acc_avg[last_idx:idx],
              self.y_acc_avg[last_idx:idx], self.z_acc_avg[last_idx:idx],
              self.x_acc_butter[last_idx:idx], self.y_acc_butter[last_idx:idx],
              self.z_acc_butter[last_idx:idx], self.norm_acc[last_idx:idx],
              self.norm_avg_acc[last_idx:idx],
              self.norm_butter_acc[last_idx:idx], self.rx_acc[last_idx:idx],
              self.ry_acc[last_idx:idx], self.rz_acc[last_idx:idx],
              self.rx_acc_avg[last_idx:idx], self.ry_acc_avg[last_idx:idx],
              self.rz_acc_avg[last_idx:idx], self.rx_acc_butter[last_idx:idx],
              self.ry_acc_butter[last_idx:idx],
              self.rz_acc_butter[last_idx:idx], self.norm_gyro[last_idx:idx],
              self.norm_avg_gyro[last_idx:idx],
              self.norm_butter_gyro[last_idx:idx]))
      last_idx = idx

    segments.append(
        PreprocessedDataSet(
            self.time_s[last_idx:], self.timestamp_ms[last_idx:],
            self.x_acc[last_idx:], self.y_acc[last_idx:],
            self.z_acc[last_idx:], self.x_acc_avg[last_idx:],
            self.y_acc_avg[last_idx:], self.z_acc_avg[last_idx:],
            self.x_acc_butter[last_idx:], self.y_acc_butter[last_idx:],
            self.z_acc_butter[last_idx:], self.norm_acc[last_idx:],
            self.norm_avg_acc[last_idx:], self.norm_butter_acc[last_idx:],
            self.rx_acc[last_idx:], self.ry_acc[last_idx:],
            self.rz_acc[last_idx:], self.rx_acc_avg[last_idx:],
            self.ry_acc_avg[last_idx:], self.rz_acc_avg[last_idx:],
            self.rx_acc_butter[last_idx:], self.ry_acc_butter[last_idx:],
            self.rz_acc_butter[last_idx:], self.norm_gyro[last_idx:],
            self.norm_avg_gyro[last_idx:], self.norm_butter_gyro[last_idx:]))

    return segments

  def crop_front(self, exercise_start_timestamp):
    # inclusive
    crop_index = self.timestamp_ms.index(exercise_start_timestamp)

    self.time_s = self.time_s[crop_index:]
    self.timestamp_ms = self.timestamp_ms[crop_index:]
    self.x_acc = self.x_acc[crop_index:]
    self.y_acc = self.y_acc[crop_index:]
    self.z_acc = self.z_acc[crop_index:]
    self.x_acc_avg = self.x_acc_avg[crop_index:]
    self.y_acc_avg = self.y_acc_avg[crop_index:]
    self.z_acc_avg = self.z_acc_avg[crop_index:]
    self.x_acc_butter = self.x_acc_butter[crop_index:]
    self.y_acc_butter = self.y_acc_butter[crop_index:]
    self.z_acc_butter = self.z_acc_butter[crop_index:]
    self.norm_acc = self.norm_acc[crop_index:]
    self.norm_avg_acc = self.norm_avg_acc[crop_index:]
    self.norm_butter_acc = self.norm_butter_acc[crop_index:]
    self.rx_acc = self.rx_acc[crop_index:]
    self.ry_acc = self.ry_acc[crop_index:]
    self.rz_acc = self.rz_acc[crop_index:]
    self.rx_acc_avg = self.rx_acc_avg[crop_index:]
    self.ry_acc_avg = self.ry_acc_avg[crop_index:]
    self.rz_acc_avg = self.rz_acc_avg[crop_index:]
    self.rx_acc_butter = self.rx_acc_butter[crop_index:]
    self.ry_acc_butter = self.ry_acc_butter[crop_index:]
    self.rz_acc_butter = self.rz_acc_butter[crop_index:]
    self.norm_gyro = self.norm_gyro[crop_index:]
    self.norm_avg_gyro = self.norm_avg_gyro[crop_index:]
    self.norm_butter_gyro = self.norm_butter_gyro[crop_index:]

  def crop_back(self, exercise_end_timestamp):
    # exclusive
    crop_index = self.timestamp_ms.index(exercise_end_timestamp)

    self.time_s = self.time_s[:crop_index]
    self.timestamp_ms = self.timestamp_ms[:crop_index]
    self.x_acc = self.x_acc[:crop_index]
    self.y_acc = self.y_acc[:crop_index]
    self.z_acc = self.z_acc[:crop_index]
    self.x_acc_avg = self.x_acc_avg[:crop_index]
    self.y_acc_avg = self.y_acc_avg[:crop_index]
    self.z_acc_avg = self.z_acc_avg[:crop_index]
    self.x_acc_butter = self.x_acc_butter[:crop_index]
    self.y_acc_butter = self.y_acc_butter[:crop_index]
    self.z_acc_butter = self.z_acc_butter[:crop_index]
    self.norm_acc = self.norm_acc[:crop_index]
    self.norm_avg_acc = self.norm_avg_acc[:crop_index]
    self.norm_butter_acc = self.norm_butter_acc[:crop_index]
    self.rx_acc = self.rx_acc[:crop_index]
    self.ry_acc = self.ry_acc[:crop_index]
    self.rz_acc = self.rz_acc[:crop_index]
    self.rx_acc_avg = self.rx_acc_avg[:crop_index]
    self.ry_acc_avg = self.ry_acc_avg[:crop_index]
    self.rz_acc_avg = self.rz_acc_avg[:crop_index]
    self.rx_acc_butter = self.rx_acc_butter[:crop_index]
    self.ry_acc_butter = self.ry_acc_butter[:crop_index]
    self.rz_acc_butter = self.rz_acc_butter[:crop_index]
    self.norm_gyro = self.norm_gyro[:crop_index]
    self.norm_avg_gyro = self.norm_avg_gyro[:crop_index]
    self.norm_butter_gyro = self.norm_butter_gyro[:crop_index]
