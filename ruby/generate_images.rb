require 'rbconfig'
require 'etc'
require 'optparse'
require_relative 'modules/system'

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

options = {}

OptionParser.new do |opt|
  opt.on('--filter_sample_count=SAMPLE_COUNT',
         'Filter count for the moving average filter') do |o|
    options[:filter_sample_count] = o
  end
end.parse!

filter_sample_count = options[:filter_sample_count]

moving_average_filter_option = if filter_sample_count.nil?
                                 '--no-moving_average_filter'
                               else
                                 '--moving_average_filter'
                               end

filter_sample_count_option = if filter_sample_count.nil?
                               '0'
                             else
                               filter_sample_count.to_s
                             end

working_directory = Dir.pwd
plotter = "#{working_directory}/python/mogasens_plotter/main.py"

def python_interpreter
  if System.os == :linux
    'python3'
  elsif System.os == :windows
    'python.exe'
  else
    STDERR.puts('Unsupported operating system, exiting.')
    exit(1)
  end
end

Dir.chdir('resources')

csv_files = Dir['./**/*.csv'].select do |file|
  file.end_with?('_out.csv')
end

other_csv_files = Dir['./**/*.csv'].reject { |file| file.end_with?('_out.csv') }

if csv_files.size != other_csv_files.size
  STDERR.puts('Unexpected amount of _out.csv files, exiting.')
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
        run_string = "#{python_interpreter} #{plotter} "\
                     "#{moving_average_filter_option} "\
                     "#{csv_file} #{sensor} "\
                     "#{imu} #{filter_sample_count_option} "\
                     "> #{dev_null}"

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
