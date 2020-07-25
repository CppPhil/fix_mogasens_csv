#!/usr/bin/env python3
# -*- coding: utf8 -*-


def sensor_to_string(sensor_id):
  left_arm_sensor_id = 769
  belly_sensor_id = 770
  right_arm_sensor_id = 771
  chest_sensor_id = 772

  if sensor_id == left_arm_sensor_id:
    return "left arm sensor"
  elif sensor_id == belly_sensor_id:
    return "belly sensor"
  elif sensor_id == right_arm_sensor_id:
    return "right arm sensor"
  elif sensor_id == chest_sensor_id:
    return "chest sensor"
  else:
    return f"bogus sensor, id: {sensor_id}"
