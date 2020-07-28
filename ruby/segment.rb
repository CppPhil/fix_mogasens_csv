require 'optparse'
require_relative 'modules/python'
require_relative 'generate_images'

options = {}

OptionParser.new do |opt|
  opt.on('--csv_file_path', 'Path to the CSV file to segment.') do |o|
    options[:csv_file_path] = o
  end

  opt.on('--sensor', 'The sensor to use (769 | 770 | 771 | 772)') do |o|
    options[:sensor] = o
  end

  opt.on('--channel', 'The channel to use (1 | 2 | 3 | 4 | 5 | 6)') do |o|
    options[:channel] = o
  end

  opt.on('--segmentation_kind',\
         'The segmentation kind to use (min | max | both)') do |o|
    options[:segmentation_kind] = o
  end

  opt.on('--out_dir', 'Path to the directory to write the segments to.') do |o|
    options[:out_dir] = o
  end
end

SEGMENTOR = "#{Dir.pwd}/python/segment.py".freeze

run_segment_py_string = \
  "#{Python.interpreter} #{SEGMENTOR} "\
  "--csv_file_path #{options[:csv_file_path]} "\
  "--sensor #{options[:sensor]} "\
  "--channel #{options[:channel]} "\
  "--segmentation_kind #{options[:segmentation_kind]} "\
  "--out_dir #{options[:out_dir]}"

unless system(run_segment_py_string)
  STDERR.puts("\"#{run_segment_py_string}\" failed, exiting.")
  exit(1)
end

exit_status = GenerateImages.main({ filter_sample_count: 0 },\
                                  options[:out_dir], false)

if exit_status != 0
  STDERR.puts('Failure invoking GenerateImages.main from segment.rb!')
end

exit(exit_status)
