#!/usr/bin/env python3
# -*- coding: utf8 -*-

from .segmentation_kind import SegmentationKind


def is_greater(value, window):
  return all([value > val for val in window])


def is_lesser(value, window):
  return all([value < val for val in window])


def local_extrema(data, window_size, segmentationKind):
  indices = []  # result
  stride = (window_size - 1) / 2
  data_size = len(data)
  max_index = data_size - 1

  for current_index in range(data_size):
    left = current_index - stride

    # Clamp to lowest index
    if current_index < stride:
      left = 0

    right = current_index + stride

    # Clamp to maximum index
    if (current_index + stride) > max_index:
      right = max_index

    current_value = data[current_index]
    current_window = data[left:right + 1]

    alreadyAdded = False

    if segmentationKind & SegmentationKind.LOCAL_MINIMA and is_lesser(
        current_value, current_window):
      indices.append(current_index)
      alreadyAdded = True

    if segmentationKind & SegmentationKind.LOCAL_MAXIMA and not alreadyAdded and is_greater(
        current_value, current_window):
      indices.append(current_index)

  return indices
