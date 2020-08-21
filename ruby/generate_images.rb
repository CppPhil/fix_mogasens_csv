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
exit(exit_status)
