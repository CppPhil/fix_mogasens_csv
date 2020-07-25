#!/usr/bin/env python3
# -*- coding: utf8 -*-

from .constants import accelerometer_string, gyroscope_string


def minimum_plotting_value(the_imu):
  if the_imu == accelerometer_string():
    return -2.0
  elif the_imu == gyroscope_string():
    return -350.0
  else:
    raise Exception('min_value: invalid imu passed')


def maximum_plotting_value(the_imu):
  if the_imu == accelerometer_string():
    return 2.0
  elif the_imu == gyroscope_string():
    return 350.0
  else:
    raise Exception('max_value: invalid imu passed')
