require 'optparse'
require_relative 'modules/system'

options = {}

OptionParser.new do |opt|
  opt.on('--skip_existing=BOOLEAN',
         'Whether to skip already existing log files') do |o|
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
PREPROCESSED_DIR = "#{RESOURCE_DIR}/preprocessed/Interpolated".freeze

CSV_FILES = Dir["#{PREPROCESSED_DIR}/*.csv"]

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
current_file_count = 0

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
              current_file_count += 1
              puts("Skipping log file #{current_file_count}/#{total_file_count} (\"#{file}\").")
              next
            end

            File.delete(file) if File.file?(file)

            CSV_FILES.each do |csv_file|
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

              current_file_count += 1
              puts("Done with log file #{current_file_count}/#{total_file_count} (\"#{file}\").")
            end
          end
        end
      end
    end
  end
end

puts('Done.')
exit(0)
