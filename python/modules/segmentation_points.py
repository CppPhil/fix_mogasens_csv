#!/usr/bin/env python3
# -*- coding: utf8 -*-
import numpy as np
from scipy.signal import argrelextrema

from .segmentation_kind import SegmentationKind


def segmentation_points(normed_data, segmentation_kind, window_size):
  data = np.array(normed_data)
  radius = int((window_size - 1) / 2)
  result = []

  # TODO: Fix overlapping of these windows.
  if segmentation_kind & SegmentationKind.LOCAL_MINIMA:
    result.extend(argrelextrema(data, np.less, order=radius)[0].tolist())

  if segmentation_kind & SegmentationKind.LOCAL_MAXIMA:
    result.extend(argrelextrema(data, np.greater, order=radius)[0].tolist())

  result.sort()

  return result
