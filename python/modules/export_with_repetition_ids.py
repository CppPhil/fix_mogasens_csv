#!/usr/bin/env python3
# -*- coding: utf8 -*-
import csv


def create_repetition_ids(preprocessed_data_set,
                          segmenting_hardware_timestamps):
  repetition_ids = []
  repetition_id = 0  # Index into segmenting_hardware_timestamps

  for hardware_timestamp in preprocessed_data_set.timestamp_ms:
    segmentation_point = segmenting_hardware_timestamps[repetition_id]

    if hardware_timestamp >= segmentation_point:
      repetition_id += 1

    repetition_ids.append(repetition_id +
                          1)  # Convert to one based index for the file

  return repetition_ids


def export_with_repetition_ids(preprocessed_data_set,
                               segmenting_hardware_timestamps, file_path):
  repetition_ids = create_repetition_ids(preprocessed_data_set,
                                         segmenting_hardware_timestamps)

  with open(file_path, 'w', newline='\n', encoding='utf-8') as csv_file:
    writer = csv.writer(csv_file,
                        delimiter=',',
                        quotechar='"',
                        quoting=csv.QUOTE_MINIMAL)
    writer.writerow([
        'RepetitionID', 'time_s', 'timestamp_ms', 'xAcc', 'yAcc', 'zAcc',
        'xAccAvg', 'yAccAvg', 'zAccAvg', 'xAccButter', 'yAccButter',
        'zAccButter', 'normAcc', 'normAvgAcc', 'normButterAcc', 'rxAcc',
        'ryAcc', 'rzAcc', 'rxAccAvg', 'ryAccAvg', 'rzAccAvg', 'rxAccButter',
        'ryAccButter', 'rzAccButter', 'normGyro', 'normAvgGyro',
        'normButterGyro', 'interpolated', 'outOfBounds'
    ])

    for i in range(preprocessed_data_set.size()):
      writer.writerow([
          repetition_ids[i], preprocessed_data_set.time_s[i],
          preprocessed_data_set.timestamp_ms[i],
          preprocessed_data_set.x_acc[i], preprocessed_data_set.y_acc[i],
          preprocessed_data_set.z_acc[i], preprocessed_data_set.x_acc_avg[i],
          preprocessed_data_set.y_acc_avg[i],
          preprocessed_data_set.z_acc_avg[i],
          preprocessed_data_set.x_acc_butter[i],
          preprocessed_data_set.y_acc_butter[i],
          preprocessed_data_set.z_acc_butter[i],
          preprocessed_data_set.norm_acc[i],
          preprocessed_data_set.norm_avg_acc[i],
          preprocessed_data_set.norm_butter_acc[i],
          preprocessed_data_set.rx_acc[i], preprocessed_data_set.ry_acc[i],
          preprocessed_data_set.rz_acc[i], preprocessed_data_set.rx_acc_avg[i],
          preprocessed_data_set.ry_acc_avg[i],
          preprocessed_data_set.rz_acc_avg[i],
          preprocessed_data_set.rx_acc_butter[i],
          preprocessed_data_set.ry_acc_butter[i],
          preprocessed_data_set.rz_acc_butter[i],
          preprocessed_data_set.norm_gyro[i],
          preprocessed_data_set.norm_avg_gyro[i],
          preprocessed_data_set.norm_butter_gyro[i],
          preprocessed_data_set.interpolated[i],
          preprocessed_data_set.out_of_bounds[i]
      ])
