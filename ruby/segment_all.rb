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
PREPROCESSED_DIR = "#{RESOURCE_DIR}/preprocessed".freeze

CSV_FILES = [
  "#{PREPROCESSED_DIR}/2020-07-02_11.17.39_Belly.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_11.17.39_Chest.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_11.17.39_LeftArm.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_11.17.39_RightArm.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_12.50.00_Belly.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_12.50.00_Chest.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_12.50.00_LeftArm.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_12.50.00_RightArm.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_13.00.09_Belly.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_13.00.09_Chest.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_13.00.09_LeftArm.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_13.00.09_RightArm.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_14.07.33_Belly.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_14.07.33_Chest.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_14.07.33_LeftArm.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_14.07.33_RightArm.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_14.14.32_Belly.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_14.14.32_Chest.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_14.14.32_LeftArm.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_14.14.32_RightArm.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_14.20.28_Belly.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_14.20.28_Chest.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_14.20.28_LeftArm.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_14.20.28_RightArm.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_14.38.40 (FalschausfБhrung)_Belly.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_14.38.40 (FalschausfБhrung)_Chest.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_14.38.40 (FalschausfБhrung)_LeftArm.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_14.38.40 (FalschausfБhrung)_RightArm.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_14.59.59_Belly.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_14.59.59_Chest.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_14.59.59_LeftArm.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_14.59.59_RightArm.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_15.13.22_Belly.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_15.13.22_Chest.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_15.13.22_LeftArm.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_15.13.22_RightArm.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_15.31.36_Belly.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_15.31.36_Chest.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_15.31.36_LeftArm.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_15.31.36_RightArm.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_15.39.22 (FalschausfБhrung)_Belly.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_15.39.22 (FalschausfБhrung)_Chest.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_15.39.22 (FalschausfБhrung)_LeftArm.csv",
  "#{PREPROCESSED_DIR}/2020-07-02_15.39.22 (FalschausfБhrung)_RightArm.csv"
].freeze

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

            next if skip_existing && File.file?(file)

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
              next if system(run_string)
              STDERR.puts("Failure invoking #{run_string}!")
              exit(1)
            end
          end
        end
      end
    end
  end
end

puts('Done.')
exit(0)
