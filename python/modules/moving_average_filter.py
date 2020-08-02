#!/usr/bin/env python3
# -*- coding: utf8 -*-


def calculate_sum(index, sample_count, input_signal):
  value = 0.0
  begin = index
  end = index + sample_count
  for i in range(begin, end):
    if len(input_signal) <= i:
      break
    value += float(input_signal[i])
  return value


def moving_average_filter(sample_count, input_signal):
  output_signal = list(range(len(input_signal)))

  for i in range(len(input_signal)):
    output_signal[i] = calculate_sum(i, sample_count,
                                     input_signal) / float(sample_count)

  return output_signal
