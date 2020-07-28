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

DEFAULT_DIRECTORY = './resources/**/*.csv'.freeze

module GenerateImages
  def self.main(options, directory, split_time_based)
    moving_average_filter_option \
   = AverageFilter.moving_average_filter_option(options)

    filter_sample_count_option\
      = AverageFilter.filter_sample_count_option(options)

    time_based_split_option = if split_time_based
                                '--time_based_split'
                              else
                                '--no-time_based_split'
                              end

    working_directory = Dir.pwd
    plotter = "#{working_directory}/python/plotter.py"

    csv_files = Dir[directory].select do |file|
      if directory == DEFAULT_DIRECTORY
        file.end_with?('_out.csv')
      else
        true
      end
    end

    if csv_files.size.zero?
      STDERR.puts('No files were found, exiting.')
      return 1
    end

    sensors = [
      769, # right arm
      770, # belly
      771, # chest
      772  # left arm
    ].freeze

    imus = %w[accelerometer gyroscope].freeze

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
              "#{time_based_split_option} "\
              "#{csv_file} "\
              "#{sensor} "\
              "#{imu} "\
              "#{filter_sample_count_option} "\
              "> #{dev_null}"

            unless system(run_string)
              STDERR.puts("\"#{run_string}\" failed, exiting.")
              return 1
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
  end
end

options = CommandLine.parse([CommandLine.filter_sample_count_option])
exit_status = GenerateImages.main(options, DEFAULT_DIRECTORY, true)
exit(exit_status)
