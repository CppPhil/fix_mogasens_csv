require 'optparse'
require 'rbconfig'
require_relative 'modules/system'
require_relative 'module/util'

MINGW_COMPILER = 'MinGW'.freeze
MSVC_COMPILER = 'MSVC'.freeze

options = {}

OptionParser.new do |opt|
  opt.on('--build_type=BUILD_TYPE', 'Debug | Release') do |o|
    options[:build_type] = o
  end
  opt.on('--compiler=COMPILER', "#{MINGW_COMPILER} | #{MSVC_COMPILER}") do |o|
    options[:compiler] = o
  end
  opt.on_tail('-h', '--help', 'Show this message') do
    STDERR.puts opt
    exit(0)
  end
end.parse!

build_type = Util.build_type(options)

compiler = options[:compiler]
compiler = MINGW_COMPILER if compiler.nil?

RESOURCES_DIR = 'resources'.freeze

def counting_app(build_type, compiler)
  if System.os == :linux
    './build/counting/counting_app'
  elsif System.os == :windows
    if compiler == MINGW_COMPILER
      'build/counting/counting_app.exe'
    elsif compiler == MSVC_COMPILER
      "build/counting/#{build_type}/counting_app.exe"
    else
      STDERR.puts("Unsupported compiler: \"#{compiler}\", exiting.")
      exit(1)
    end
  else
    STDERR.puts('Unrecognized operating system, exiting.')
    exit(1)
  end
end

csv_files = Dir["#{RESOURCES_DIR}/**/*.csv"].select do |file|
  file.end_with?('_out.csv')
end

if csv_files.empty?
  STDERR.puts('No _out.csv files were found, exiting.')
  exit(1)
end

csv_files.map! { |file| "\"#{file}\"" }

unless system("#{counting_app(build_type, compiler)} #{csv_files.join(' ')}")
  STDERR.puts('Failure running counting_app, exiting.')
  exit(1)
end

exit(0)
