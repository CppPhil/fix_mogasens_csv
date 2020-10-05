require 'optparse'
require_relative 'modules/python'
require_relative 'modules/generate_images_module'

options = {}

OptionParser.new do |opt|
  opt.on('--skip_window=BOOLEAN',
         'Whether to skip over the remainder of the window when a segmentation point is found') do |o|
    options[:skip_window] = (o.casecmp 'true').zero?
  end

  opt.on('--delete_too_close=BOOLEAN',
         'Whether to delete too close segmentation points') do |o|
    options[:delete_too_close] = (o.casecmp 'true').zero?
  end

  opt.on('--delete_low_variance=BOOLEAN',
         'Whether to delete segments with too low variance') do |o|
    options[:delete_low_variance] = (o.casecmp 'true').zero?
  end

  opt.on('--image_format=IMAGE_FORMAT',
         'The image format to use e.g. svg; defaults to png') do |o|
    options[:image_format] = o
  end

  opt.on('--csv_file_path=PATH', 'Path to the CSV file to segment.') do |o|
    options[:csv_file_path] = o
  end

  opt.on('--imu=IMU',\
         'The IMU to use (accelerometer | gyroscope)') do |o|
    options[:imu] = o
  end

  opt.on('--segmentation_kind=KIND',\
         'The segmentation kind to use (min | max | both)') do |o|
    options[:segmentation_kind] = o
  end

  opt.on('--window_size=WINDOW_SIZE',\
         'The window size to use for segmenting') do |o|
    options[:window_size] = o
  end

  opt.on_tail('-h', '--help', 'Show this message') do
    STDERR.puts opt
    exit(0)
  end
end.parse!

image_format = if options[:image_format].nil?
                 'png'
               else
                 options[:image_format]
               end

skip_window = if options[:skip_window]
                '--skip_window'
              else
                '--no-skip_window'
              end

delete_too_close = if options[:delete_too_close]
                     '--delete_too_close'
                   else
                     '--no-delete_too_close'
                   end

delete_low_variance = if options[:delete_low_variance]
                        '--delete_low_variance'
                      else
                        '--no-delete_low_variance'
                      end

puts("preprocessed_segment.rb: Starting.\n"\
     "command line options: \"#{options}\"\n"\
     "image_format is \"#{image_format}\".")

SEGMENTOR = "\"#{Dir.pwd}/python/preprocessed_segment.py\"".freeze

run_string = \
  "#{Python.interpreter} #{SEGMENTOR} "\
  "#{skip_window} "\
  "#{delete_too_close} "\
  "#{delete_low_variance} "\
  "--image_format #{image_format} "\
  "--csv_file_path \"#{options[:csv_file_path]}\" "\
  "--imu #{options[:imu]} "\
  "--segmentation_kind #{options[:segmentation_kind]} "\
  "--window_size #{options[:window_size]}"

unless system(run_string)
  STDERR.puts("\"#{run_string}\" failed, exiting.")
  exit(1)
end

exit(0)
