#!/usr/bin/env python3
# -*- coding: utf8 -*-

from .sensors import *


def sensor_to_string(sensor_id):
  if sensor_id == left_arm_sensor_id():
    return "left arm sensor"
  elif sensor_id == belly_sensor_id():
    return "belly sensor"
  elif sensor_id == right_arm_sensor_id():
    return "right arm sensor"
  elif sensor_id == chest_sensor_id():
    return "chest sensor"
  else:
    return f"bogus sensor, id: {sensor_id}"
