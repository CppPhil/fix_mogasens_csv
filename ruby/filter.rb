require_relative 'modules/command_line'
require_relative 'modules/average_filter'
require_relative 'modules/generate_images_module'
require_relative 'modules/python'

options = CommandLine.parse([CommandLine.filter_sample_count_option])
moving_average_filter_option \
   = AverageFilter.moving_average_filter_option(options)

filter_sample_count_option\
      = AverageFilter.filter_sample_count_option(options)

puts("filter.rb: Starting.\n"\
     "moving_average_filter_option is \"#{moving_average_filter_option}\".\n"\
     "filter_sample_count_option is \"#{filter_sample_count_option}\".")

FILTERER = "\"#{Dir.pwd}/python/filter.py\"".freeze

csv_files = Dir[GenerateImagesModule.default_directory].select do |file|
  file.end_with?('_out.csv')
end

if csv_files.size.zero?
  STDERR.puts('No files were found, exiting.')
  exit(1)
end

RUN_STRING = "#{Python.interpreter} #{FILTERER} "\
             "#{moving_average_filter_option} "\
             '--csv_file_path "%s" '\
             "#{filter_sample_count_option}".freeze

csv_files.each do |file|
  run_string = format(RUN_STRING, file)
  puts("Applying #{filter_sample_count_option} filter to \"#{file}\".")
  unless system(run_string)
    STDERR.puts("\"#{run_string}\" failed, exiting.")
    exit(1)
  end
end

puts('Done.')
exit(0)
