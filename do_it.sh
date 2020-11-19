#!/bin/bash

ARRAY=()

for file in ./resources/preprocessed/Interpolated-Revised/*.csv; do
  ARRAY+=($file)
done

for e in "${ARRAY[@]}"; do
  ./preprocessed_segment.sh --skip_window=false --delete_too_close=false --delete_low_variance=false --image_format=png --csv_file_path=$file --imu=accelerometer --segmentation_kind=max --window_size=501 --filter=butterworth
done

exit 0

