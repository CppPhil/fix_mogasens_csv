#!/usr/bin/env python3
# -*- coding: utf8 -*-

from enum import Flag, auto


class SegmentationKind(Flag):
  LOCAL_MINIMA = auto()
  LOCAL_MAXIMA = auto()


def segmentation_kind_from_str(string):
  if string == 'min':
    return SegmentationKind.LOCAL_MINIMA

  if string == 'max':
    return SegmentationKind.LOCAL_MAXIMA

  if string == 'both':
    return SegmentationKind.LOCAL_MINIMA | SegmentationKind.LOCAL_MAXIMA

  raise Exception(f"invalid input \"{string}\" to segmentation_kind_from_str")
