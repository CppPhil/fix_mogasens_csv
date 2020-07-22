#!/usr/bin/env python3
# -*- coding: utf8 -*-

import pandas as pd


def moving_average_filter(sample_count, input_signal):
  df = pd.DataFrame({'y': input_signal})
  output_signal = df['y'].rolling(window=sample_count).mean().to_list()
  return output_signal
