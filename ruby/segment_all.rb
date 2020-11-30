require 'optparse'
require 'etc'
require_relative 'modules/system'

options = {}

OptionParser.new do |opt|
  opt.on('--skip_existing=BOOLEAN',
         'Whether to skip already existing log files (defaults to true)') do |o|
    options[:skip_existing] = (o.casecmp 'true').zero?
  end

  opt.on_tail('-h', '--help', 'Show this message') do
    STDERR.puts opt
    exit(0)
  end
end.parse!

skip_existing = if options[:skip_existing].nil?
                  true
                else
                  options[:skip_existing]
                end

RESOURCE_DIR = 'resources'.freeze
INTERPOLATED_DIR = "#{RESOURCE_DIR}/preprocessed/Interpolated".freeze

csv_files = Dir["#{INTERPOLATED_DIR}/*.csv"].reject do |file|
  file.include?('Jan_liegestuetzen1')
end
csv_files += Dir["#{RESOURCE_DIR}/preprocessed/Interpolated-Revised/*.csv"]

SKIP_WINDOW_OPTIONS = %w[false true].freeze
DELETE_TOO_CLOSE_OPTIONS = %w[false true].freeze
DELETE_LOW_VARIANCE_OPTIONS = %w[false true].freeze
SEGMENTATION_KINDS = %w[both max min].freeze
WINDOW_SIZES = %w[101 151 201 251 301 351 401 451 501 51 551].freeze
FILTERS = %w[average butterworth].freeze

LOG_DIR = 'segmentation_comparison/logs'.freeze

def script
  if System.os == :windows
    'preprocessed_segment.bat'
  else
    './preprocessed_segment.sh'
  end
end

total_file_count = SKIP_WINDOW_OPTIONS.size * DELETE_TOO_CLOSE_OPTIONS.size\
 * DELETE_LOW_VARIANCE_OPTIONS.size * SEGMENTATION_KINDS.size * WINDOW_SIZES.size * FILTERS.size
counter = 1
threads = []

SKIP_WINDOW_OPTIONS.each do |skip_window_option|
  DELETE_TOO_CLOSE_OPTIONS.each do |delete_too_close_option|
    DELETE_LOW_VARIANCE_OPTIONS.each do |delete_low_variance_option|
      SEGMENTATION_KINDS.each do |segmentation_kind|
        WINDOW_SIZES.each do |window_size|
          FILTERS.each do |filter|
            file = "#{LOG_DIR}/skip_window-#{skip_window_option}_delete_too_"\
                   "close-#{delete_too_close_option}_delete_low_variance-"\
                   "#{delete_low_variance_option}_kind-#{segmentation_kind}_"\
                   "window-#{window_size}_filter-#{filter}.log"

            if skip_existing && File.file?(file)
              counter += 1
              next
            end

            File.delete(file) if File.file?(file)

            threads << Thread.new do
              csv_files.each do |csv_file|
                run_string = "#{script} "\
                  "--skip_window=\"#{skip_window_option}\" "\
                  "--delete_too_close=\"#{delete_too_close_option}\" "\
                  "--delete_low_variance=\"#{delete_low_variance_option}\" "\
                  '--image_format=png '\
                  "--csv_file_path=\"#{csv_file}\" "\
                  '--imu=accelerometer '\
                  "--segmentation_kind=\"#{segmentation_kind}\" "\
                  "--window_size=\"#{window_size}\" "\
                  "--filter=\"#{filter}\" "\
                  ">> \"#{file}\""

                unless system(run_string)
                  STDERR.puts("Failure invoking #{run_string}!")
                  exit(1)
                end
              end
            end

            if (counter % Etc.nprocessors).zero?
              threads.each(&:join)
              threads.clear
            end

            printf("%.2f%%\r", counter.to_f / total_file_count.to_f * 100.0)
            $stdout.flush
            counter += 1
          end
        end
      end
    end
  end
end

threads.each(&:join)
puts('100.00%')
$stdout.flush
puts('Done.')
exit(0)
