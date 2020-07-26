require 'rbconfig'
require_relative 'modules/system'
require_relative 'modules/util'
require_relative 'modules/command_line'
require_relative 'modules/average_filter'
require_relative 'modules/python'

MINGW_COMPILER = 'MinGW'.freeze
MSVC_COMPILER = 'MSVC'.freeze

options = CommandLine.parse([CommandLine.filter_sample_count_option, \
                             CommandLine.build_type_option, \
                             CommandLine.compiler_option])

moving_average_filter_option \
 = AverageFilter.moving_average_filter_option(options)

filter_sample_count_option = AverageFilter.filter_sample_count_option(options)

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

filterer = "#{Dir.pwd}/python/filter.py"
run_string = "#{Python.interpreter} #{filterer} "\
             "#{moving_average_filter_option} "\
             "--csv_file_path \"%s\" " \
             "#{filter_sample_count_option}"

files_to_filter = Dir["#{RESOURCES_DIR}/**/*.csv"].select do |file|
  file.end_with?('_out.csv')
end

files_to_filter.each do |file|
  str = sprintf(run_string, file)
  unless system(str)
    STDERR.puts("\"#{str}\" failed, exiting.")
    exit(1)
  end
end

csv_files = Dir["#{RESOURCES_DIR}/**/*.csv"].select do |file|
  file.include?('_out') && !file.end_with?('_out.csv')
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
