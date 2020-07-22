#!/usr/bin/env python3
# -*- coding: utf8 -*-


def calculate_sum(index, sample_count, input_signal):
    value = 0.0
    begin = index
    end = index + sample_count
    for i in range(begin, end):
        if len(input_signal) <= i:
            break
        value += input_signal[i]
    return value


def moving_average_filter(sample_count, input_signal):
    for i in range(len(input_signal)):
        input_signal[i] = calculate_sum(i, sample_count,
                                        input_signal) / sample_count
