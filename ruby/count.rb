require 'optparse'
require 'rbconfig'

def os
  @os ||= begin
    host_os = RbConfig::CONFIG['host_os']
    case host_os
    when /mswin|msys|mingw|cygwin|bccwin|wince|emc/
      :windows
    when /darwin|mac os/
      :macosx
    when /linux/
      :linux
    when /solaris|bsd/
      :unix
    else
      return :unknown
    end
  end
end

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

compiler = options[:compiler]
compiler = MINGW_COMPILER if compiler.nil?

RESOURCES_DIR = 'resources'.freeze

def counting_app(build_type, compiler)
  if os == :linux
    './build/counting/counting_app'
  elsif os == :windows
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

csv_files.map! { |file| "\"#{file}\"" }

unless system("#{counting_app(build_type, compiler)} #{csv_files.join(' ')}")
  STDERR.puts('Failure running counting_app, exiting.')
  exit(1)
end

exit(0)
