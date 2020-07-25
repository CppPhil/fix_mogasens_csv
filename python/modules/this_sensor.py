#!/usr/bin/env python3
# -*- coding: utf8 -*-


def this_sensor(desired_sensor, time, hardware_timestamp, extract_id, trigger,
                accelerometer_x, accelerometer_y, accelerometer_z, gyroscope_x,
                gyroscope_y, gyroscope_z):
  this_sensor_time = []
  this_sensor_hardware_timestamp = []
  this_sensor_extract_id = []
  this_sensor_trigger = []
  this_sensor_accelerometer_x = []
  this_sensor_accelerometer_y = []
  this_sensor_accelerometer_z = []
  this_sensor_gyroscope_x = []
  this_sensor_gyroscope_y = []
  this_sensor_gyroscope_z = []

  for i in range(len(time)):
    current_sensor_id = extract_id[i]

    if desired_sensor == current_sensor_id:
      this_sensor_time.append(time[i])
      this_sensor_hardware_timestamp.append(hardware_timestamp[i])
      this_sensor_extract_id.append(extract_id[i])
      this_sensor_trigger.append(trigger[i])
      this_sensor_accelerometer_x.append(accelerometer_x[i])
      this_sensor_accelerometer_y.append(accelerometer_y[i])
      this_sensor_accelerometer_z.append(accelerometer_z[i])
      this_sensor_gyroscope_x.append(gyroscope_x[i])
      this_sensor_gyroscope_y.append(gyroscope_y[i])
      this_sensor_gyroscope_z.append(gyroscope_z[i])

  return this_sensor_time, this_sensor_hardware_timestamp, this_sensor_extract_id, this_sensor_trigger, this_sensor_accelerometer_x, this_sensor_accelerometer_y, this_sensor_accelerometer_z, this_sensor_gyroscope_x, this_sensor_gyroscope_y, this_sensor_gyroscope_z
