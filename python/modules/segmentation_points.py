#!/usr/bin/env python3
# -*- coding: utf8 -*-
from .segmentation_kind import SegmentationKind


def segmentation_points(normed_data, segmentation_kind, window_size,
                        skip_window):
  return segmentation_points_impl(data=normed_data,
                                  window_size=window_size,
                                  skip_window=skip_window,
                                  segmentation_kind=segmentation_kind)


def segmentation_points_impl(data, window_size, skip_window,
                             segmentation_kind):
  radius = int((window_size - 1) / 2)
  extremum_indices = []
  current_index = 0
  data_point_count = len(data)
  while current_index < data_point_count:
    window_begin = current_index - radius
    window_end = current_index + radius
    if window_begin < 0:
      window_begin = 0
    if window_end >= data_point_count:
      window_end = data_point_count - 1
    if segmentation_kind & SegmentationKind.LOCAL_MINIMA:
      is_local_minimum = True
      for i in range(window_begin, window_end + 1):
        if i != current_index and data[i] < data[current_index]:
          is_local_minimum = False
      if is_local_minimum:
        extremum_indices.append(current_index)
        if skip_window:
          current_index = window_end + 1
        else:
          current_index = current_index + 1
    if segmentation_kind & SegmentationKind.LOCAL_MAXIMA:
      is_local_maximum = True
      for i in range(window_begin, window_end + 1):
        if i != current_index and data[i] > data[current_index]:
          is_local_maximum = False
      if is_local_maximum:
        extremum_indices.append(current_index)
        if skip_window:
          current_index = window_end + 1
        else:
          current_index = current_index + 1
  return extremum_indices
