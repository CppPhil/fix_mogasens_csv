#!/usr/bin/env python3
# -*- coding: utf8 -*-

from enum import Flag, auto


class SegmentationKind(Flag):
  LOCAL_MINIMA = auto()
  LOCAL_MAXIMA = auto()
