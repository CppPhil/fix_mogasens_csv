require_relative 'modules/command_line'
require_relative 'modules/util'

options = CommandLine.parse([CommandLine.BUILD_TYPE_OPTION, \
                             CommandLine.HELP_OPTION])

build_type = Util.build_type(options)

Dir.chdir('build')

unless system("ctest . -C \"#{build_type}\" --output-on-failure")
  STDERR.puts('Failure running ctest, exiting.')
  exit(1)
end

exit(0)
