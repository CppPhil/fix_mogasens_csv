require_relative 'modules/command_line'
require_relative 'modules/util'

options = CommandLine.parse([CommandLine.build_type_option])

build_type = Util.build_type(options)

puts("test.rb: Starting.\n"\
     "build_type is \"#{build_type}\".")

Dir.chdir('build')

unless system("ctest . -C \"#{build_type}\" --output-on-failure")
  STDERR.puts('Failure running ctest, exiting.')
  exit(1)
end

Dir.chdir('..')
exit(0)
