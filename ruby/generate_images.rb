require_relative 'modules/command_line'
require_relative 'modules/generate_images_module'

options = CommandLine.parse(\
  [CommandLine.filter_sample_count_option, CommandLine.image_format_option]
)

puts("generate_images.rb: Starting.\n"\
     "command line options: \"#{options}\".")

exit_status = GenerateImagesModule.main(\
  options,\
  GenerateImagesModule.default_directory,\
  true
)
exit(exit_status)
