#!/usr/bin/env python3
# -*- coding: utf8 -*-

from constants import accelerometer_string, gyroscope_string


def min_value(the_imu):
  if the_imu == accelerometer_string():
    return -2.0
  elif the_imu == gyroscope_string():
    return -2000.0
  else:
    raise Exception('min_value: invalid imu passed')


def max_value(the_imu):
  if the_imu == accelerometer_string():
    return 2.0
  elif the_imu == gyroscope_string():
    return 2000.0
  else:
    raise Exception('max_value: invalid imu passed')
