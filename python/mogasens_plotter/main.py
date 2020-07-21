#!/usr/bin/env python3
# -*- coding: utf8 -*-

# Sensors
# 769 = left arm
# 770 = belly
# 771 = right arm
# 772 = chest

import argparse
import csv
import sys

import matplotlib.pyplot as plt
import pandas as pd


def hardware_timestamp_string():
    return 'hardware_timestamp'


def channel1_string():
    return 'channel1'


def channel2_string():
    return 'channel2'


def channel3_string():
    return 'channel3'


def channel4_string():
    return 'channel4'


def channel5_string():
    return 'channel5'


def channel6_string():
    return 'channel6'


def accelerometer_string():
    return 'accelerometer'


def gyroscope_string():
    return 'gyroscope'


def sensor_to_string(sensor_id):
    left_arm_sensor_id = 769
    belly_sensor_id = 770
    right_arm_sensor_id = 771
    chest_sensor_id = 772

    if sensor_id == left_arm_sensor_id:
        return "left arm sensor"
    elif sensor_id == belly_sensor_id:
        return "belly sensor"
    elif sensor_id == right_arm_sensor_id:
        return "right arm sensor"
    elif sensor_id == chest_sensor_id:
        return "chest sensor"
    else:
        return f"bogus sensor, id: {sensor_id}"


def plot(the_imu, data_frame):
    first_color = 'blue'
    second_color = 'red'
    third_color = 'green'

    first_label = 'x'
    second_label = 'y'
    third_label = 'z'

    line_width = 0.6

    def plot_channel(channel, color, label):
        plt.plot(hardware_timestamp_string(),
                 channel,
                 data=data_frame,
                 color=color,
                 label=label,
                 linewidth=line_width)

    if the_imu == accelerometer_string():
        plot_channel(channel1_string(), first_color, first_label)
        plot_channel(channel2_string(), second_color, second_label)
        plot_channel(channel3_string(), third_color, third_label)
    elif the_imu == gyroscope_string():
        plot_channel(channel4_string(), first_color, first_label)
        plot_channel(channel5_string(), second_color, second_label)
        plot_channel(channel6_string(), third_color, third_label)
    else:
        print(
            f'imu was "{the_imu}" which is neither "#{accelerometer_string()}" nor "#{gyroscope_string()}", '
            f'exiting.',
            file=sys.stderr)
        sys.exit()


def imu_unit(the_imu):
    if the_imu == accelerometer_string():
        return 'g'
    elif the_imu == gyroscope_string():
        return 'deg/s'
    else:
        return f"invalid imu: \"{the_imu}\"!"


def main():
    parser = argparse.ArgumentParser(description='Plot MoGaSens CSV file.')
    parser.add_argument('csv_file_path',
                        type=str,
                        help='Path to the CSV file to plot.')
    parser.add_argument('sensor',
                        type=int,
                        help='The sensor to plot (769 | 770 | 771 | 772).')
    parser.add_argument(
        'imu',
        type=str,
        help='The Inertial Measurement Unit to use (accelerometer / gyroscope).'
    )

    args = parser.parse_args()

    csv_file_path = args.csv_file_path
    desired_sensor = args.sensor
    imu = args.imu

    if sensor_to_string(desired_sensor).startswith('bogus sensor'):
        print(f"{desired_sensor} is not a valid sensor ID, exiting.",
              file=sys.stderr)
        sys.exit()

    time_column_index = 0
    hardware_timestamp_index = 1
    extract_id_column_index = 2
    trigger_index = 3
    accelerometer_x_column_index = 4
    accelerometer_y_column_index = 5
    accelerometer_z_column_index = 6
    gyroscope_x_column_index = 7
    gyroscope_y_column_index = 8
    gyroscope_z_column_index = 9

    time = []
    hardware_timestamp = []
    extract_id = []
    trigger = []
    accelerometer_x = []
    accelerometer_y = []
    accelerometer_z = []
    gyroscope_x = []
    gyroscope_y = []
    gyroscope_z = []

    with open(csv_file_path, 'r') as csv_file:
        plots = csv.reader(csv_file, delimiter=',')
        for row_count, row in enumerate(plots):
            if row_count == 0:  # Skip the header row
                continue

            time.append(float(row[time_column_index]))
            hardware_timestamp.append(int(row[hardware_timestamp_index]))
            extract_id.append(int(row[extract_id_column_index]))
            trigger.append(float(row[trigger_index]))
            accelerometer_x.append(float(row[accelerometer_x_column_index]))
            accelerometer_y.append(float(row[accelerometer_y_column_index]))
            accelerometer_z.append(float(row[accelerometer_z_column_index]))
            gyroscope_x.append(float(row[gyroscope_x_column_index]))
            gyroscope_y.append(float(row[gyroscope_y_column_index]))
            gyroscope_z.append(float(row[gyroscope_z_column_index]))

    hardware_timestamp_data = []
    channel1_data = []
    channel2_data = []
    channel3_data = []
    channel4_data = []
    channel5_data = []
    channel6_data = []

    hardware_timestamp_accumulator = []
    channel1_accumulator = []
    channel2_accumulator = []
    channel3_accumulator = []
    channel4_accumulator = []
    channel5_accumulator = []
    channel6_accumulator = []

    hardware_timestamp_threshold_offset = 10000  # 10 seconds
    hardware_timestamp_threshold = hardware_timestamp[
        0] + hardware_timestamp_threshold_offset

    def append_accumulator(data, accumulator):
        data.append(accumulator.copy())
        accumulator.clear()

    def append_accumulators():
        append_accumulator(hardware_timestamp_data,
                           hardware_timestamp_accumulator)
        append_accumulator(channel1_data, channel1_accumulator)
        append_accumulator(channel2_data, channel2_accumulator)
        append_accumulator(channel3_data, channel3_accumulator)
        append_accumulator(channel4_data, channel4_accumulator)
        append_accumulator(channel5_data, channel5_accumulator)
        append_accumulator(channel6_data, channel6_accumulator)

    for i in range(len(time)):
        current_sensor_id = extract_id[i]

        if current_sensor_id == desired_sensor:
            hardware_timestamp_accumulator.append(hardware_timestamp[i])
            channel1_accumulator.append(accelerometer_x[i])
            channel2_accumulator.append(accelerometer_y[i])
            channel3_accumulator.append(accelerometer_z[i])
            channel4_accumulator.append(gyroscope_x[i])
            channel5_accumulator.append(gyroscope_y[i])
            channel6_accumulator.append(gyroscope_z[i])

        current_hardware_timestamp = hardware_timestamp[i]

        if current_hardware_timestamp >= hardware_timestamp_threshold:
            append_accumulators()
            hardware_timestamp_threshold += hardware_timestamp_threshold_offset

    if len(hardware_timestamp_accumulator) != 0:
        append_accumulators()

    for i in range(len(hardware_timestamp_data)):
        df = pd.DataFrame({
            hardware_timestamp_string():
            hardware_timestamp_data[i],
            channel1_string():
            channel1_data[i],
            channel2_string():
            channel2_data[i],
            channel3_string():
            channel3_data[i],
            channel4_string():
            channel4_data[i],
            channel5_string():
            channel5_data[i],
            channel6_string():
            channel6_data[i]
        })

        title = f"{csv_file_path}_{sensor_to_string(desired_sensor)}_{imu}_{i + 1}".replace(
            " ", "_")
        png_file = f"{title}.png"

        x_size = 11
        y_size = 6
        plt.rc('figure', figsize=(x_size, y_size))
        plot(imu, df)
        plt.legend(loc='upper right')

        plt.title(title)
        plt.ylabel(imu_unit(imu))
        plt.xlabel(f'{hardware_timestamp_string()} (in milliseconds)')
        plt.gca().xaxis.set_major_locator(
            plt.MultipleLocator(1000))  # A second
        plt.grid()
        plt.savefig(png_file, bbox_inches='tight')
        plt.close()

        print(f"Wrote \"{png_file}\".")


if __name__ == "__main__":
    main()
