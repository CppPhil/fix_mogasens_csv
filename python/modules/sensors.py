#!/usr/bin/env python3
# -*- coding: utf8 -*-


def left_arm_sensor_id():
  return 769


def belly_sensor_id():
  return 770


def right_arm_sensor_id():
  return 771


def chest_sensor_id():
  return 772


def sensors():
  return [
      left_arm_sensor_id(),
      belly_sensor_id(),
      right_arm_sensor_id(),
      chest_sensor_id()
  ]
