#!/usr/bin/env python3
# -*- coding: utf8 -*-

import math
import sys


def check_length(x_length, y_length, z_length):
  if not all(value == x_length for value in [x_length, y_length, z_length]):
    print(f"euclidean_norm.py: vectors were not of equal lengths!",
          file=sys.stderr)
    print(f"euclidean_norm.py: Exiting with status code 1!", file=sys.stderr)
    sys.exit(1)


def euclidean_norm(x, y, z):
  check_length(len(x), len(y), len(z))

  result = [0.0] * len(x)

  for i in range(len(result)):
    result[i] = math.sqrt((x[i]**2.0) + (y[i]**2.0) + (z[i]**2.0))

  return result
