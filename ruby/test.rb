require 'optparse'
require_relative 'modules/util'

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

build_type = Util.build_type(options)

Dir.chdir('build')

unless system("ctest . -C \"#{build_type}\" --output-on-failure")
  STDERR.puts('Failure running ctest, exiting.')
  exit(1)
end

exit(0)
