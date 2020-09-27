#!/usr/bin/env python3
# -*- coding: utf8 -*-
from .segmentation_kind import SegmentationKind


def find_local_extrema(data, segmentation_kind, radius):
  extrema = []
  current_index = 0

  while current_index < len(data):
    # Closed range
    window_begin = current_index - radius
    window_end = current_index + radius

    # Clamp
    if window_begin < 0:
      window_begin = 0
    if window_end >= len(data):
      window_end = len(data) - 1

    window = data[window_begin:current_index] + data[current_index +
                                                     1:window_end + 1]
    current_value = data[current_index]

    if ((segmentation_kind & SegmentationKind.LOCAL_MINIMA) and all(current_value < element for element in window)) \
       or ((segmentation_kind & SegmentationKind.LOCAL_MAXIMA) and all(current_value > element for element in window)):
      extrema.append(current_index)
      current_index = window_end + 1  # Skip over the window after having found a value.
    else:
      current_index += 1

  return extrema


def segmentation_points(normed_data, segmentation_kind, window_size):
  radius = int((window_size - 1) / 2)
  return find_local_extrema(normed_data, segmentation_kind, radius)
