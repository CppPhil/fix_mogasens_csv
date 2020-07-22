#!/usr/bin/env python3
# -*- coding: utf8 -*-

from constants import accelerometer_string, gyroscope_string


def imu_unit(the_imu):
  if the_imu == accelerometer_string():
    return 'g'
  elif the_imu == gyroscope_string():
    return 'deg/s'
  else:
    return f"invalid imu: \"{the_imu}\"!"
