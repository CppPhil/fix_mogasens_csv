require 'optparse'

options = {}

OptionParser.new do |opt|
  opt.on('--build_type=BUILD_TYPE', 'Debug | Release') do |o|
    options[:build_type] = o
  end
  opt.on_tail('-h', '--help', 'Show this message') do
    STDERR.puts opt
    exit(0)
  end
end.parse!

build_type = options[:build_type]
DEBUG_BUILD_TYPE = 'Debug'.freeze
RELEASE_BUILD_TYPE = 'Release'.freeze
build_type = RELEASE_BUILD_TYPE if build_type.nil?

if build_type != DEBUG_BUILD_TYPE && build_type != RELEASE_BUILD_TYPE
  STDERR.puts("Invalid build type \"#{build_type}\"! "\
              "Allowable values are \"#{DEBUG_BUILD_TYPE}\" "\
              "and \"#{RELEASE_BUILD_TYPE}\".")
  exit(1)
end

Dir.chdir('build')

unless system("ctest . -C \"#{build_type}\" --output-on-failure")
  STDERR.puts('Failure running ctest, exiting.')
  exit(1)
end

exit(0)
