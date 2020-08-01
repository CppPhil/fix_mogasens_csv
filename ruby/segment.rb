require 'optparse'
require_relative 'modules/python'
require_relative 'modules/generate_images_module'

options = {}

OptionParser.new do |opt|
  opt.on('--csv_file_path=PATH', 'Path to the CSV file to segment.') do |o|
    options[:csv_file_path] = o
  end

  opt.on('--sensor=SENSOR',\
         'The sensor to use for segmenting (769 | 770 | 771 | 772)') do |o|
    options[:sensor] = o
  end

  opt.on('--channel=CHANNEL',\
         'The channel to use for segmenting (1 | 2 | 3 | 4 | 5 | 6)') do |o|
    options[:channel] = o
  end

  opt.on('--segmentation_kind=KIND',\
         'The segmentation kind to use (min | max | both)') do |o|
    options[:segmentation_kind] = o
  end

  opt.on_tail('-h', '--help', 'Show this message') do
    STDERR.puts opt
    exit(0)
  end
end.parse!

SEGMENTOR = "#{Dir.pwd}/python/segment.py".freeze

run_segment_py_string = \
  "#{Python.interpreter} #{SEGMENTOR} "\
  "--csv_file_path #{options[:csv_file_path]} "\
  "--sensor #{options[:sensor]} "\
  "--channel #{options[:channel]} "\
  "--segmentation_kind #{options[:segmentation_kind]}"

unless system(run_segment_py_string)
  STDERR.puts("\"#{run_segment_py_string}\" failed, exiting.")
  exit(1)
end

exit(0)
