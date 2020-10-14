require_relative 'modules/command_line'
require_relative 'modules/generate_images_module'
require_relative 'modules/ruby'

options = CommandLine.parse(\
  [CommandLine.filter_sample_count_option, CommandLine.image_format_option]
)

puts("generate_images.rb: Starting.\n"\
     "command line options: \"#{options}\".")

FILTER_SCRIPT = "\"#{Dir.pwd}/ruby/filter.rb\"".freeze
RUN_STRING = "#{Ruby.interpreter} #{FILTER_SCRIPT}"\
             " --filter_sample_count=#{options[:filter_sample_count]}".freeze

unless options[:filter_sample_count].nil?
  unless system(RUN_STRING)
    STDERR.puts("\"#{RUN_STRING}\" failed, exiting.")
    exit(1)
  end
end

exit_status = GenerateImagesModule.main(\
  options,\
  GenerateImagesModule.default_directory,\
  true
)

exit(exit_status) if exit_status != 0

puts('Starting to visualize the interpolated data sets...')

csvs = Dir['./resources/preprocessed/Interpolated/*.csv']
imus = %w[accelerometer gyroscope].freeze
counter = 1
threads = []
image_format = if options[:image_format].nil?
                 'png'
               else
                 options[:image_format]
               end
total_count = csvs.size * imus.size

csvs.each do |csv|
  imus.each do |imu|
    threads << Thread.new do
      run_string = \
        "#{Python.interpreter} python/run_preprocessed_plotter.py "\
        "--image_format #{image_format} "\
        "\"#{csv}\" "\
        "#{imu} "\
        "> #{GenerateImagesModule.dev_null}"

      unless system(run_string)
        STDERR.puts("\"#{run_string}\" failed, exiting.")
        exit(1)
      end
    end

    if (counter % Etc.nprocessors).zero?
      threads.each(&:join)
      threads.clear
    end

    printf("%.2f%%\r", counter.to_f / total_count.to_f * 100.0)
    $stdout.flush
    counter += 1
  end
end

threads.each(&:join)
puts('100.00%')
$stdout.flush
puts('Done visualizing the interpolated data sets.')
exit(0)
