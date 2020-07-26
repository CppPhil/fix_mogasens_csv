require 'rbconfig'
require 'etc'
require_relative 'modules/python'
require_relative 'modules/command_line'
require_relative 'modules/average_filter'

def dev_null
  if System.os == :linux
    '/dev/null'
  elsif System.os == :windows
    'NUL'
  else
    STDERR.puts('Unsupported operating system, exiting.')
    exit(1)
  end
end

options = CommandLine.parse([CommandLine.filter_sample_count_option])

moving_average_filter_option \
 = AverageFilter.moving_average_filter_option(options)

filter_sample_count_option = AverageFilter.filter_sample_count_option(options)

working_directory = Dir.pwd
plotter = "#{working_directory}/python/plotter.py"

Dir.chdir('resources')

csv_files = Dir['./**/*.csv'].select do |file|
  file.end_with?('_out.csv')
end

if csv_files.size.zero?
  STDERR.puts('No _out.csv files were found, exiting.')
  exit(1)
end

sensors = [
  769, # right arm
  770, # belly
  771, # chest
  772  # left arm
]

imus = %w[accelerometer gyroscope]

proc_count = Etc.nprocessors
total_count = csv_files.size * sensors.size * imus.size
counter = 1
threads = []

puts("Generating images with #{proc_count} threads.")

csv_files.each do |csv_file|
  sensors.each do |sensor|
    imus.each do |imu|
      threads << Thread.new do
        run_string = \
          "#{Python.interpreter} #{plotter} "\
          "#{moving_average_filter_option} "\
          "#{csv_file} "\
          "#{sensor} "\
          "#{imu} "\
          "#{filter_sample_count_option}"

        unless system(run_string)
          STDERR.puts("\"#{run_string}\" failed, exiting.")
          exit(1)
        end
      end

      if (counter % proc_count).zero?
        threads.each(&:join)
        threads.clear
      end

      printf("%.2f%%\r", counter.to_f / total_count.to_f * 100.0)
      $stdout.flush
      counter += 1
    end
  end
end

threads.each(&:join)
puts('100.00%')
$stdout.flush

puts('Done.')

exit(0)
