require 'rbconfig'
require_relative 'modules/system'
require_relative 'modules/util'
require_relative 'modules/command_line'

MINGW_COMPILER = 'MinGW'.freeze
MSVC_COMPILER = 'MSVC'.freeze

options = CommandLine.parse([CommandLine.build_type_option, \
                             CommandLine.compiler_option])

build_type = Util.build_type(options)

compiler = options[:compiler]
compiler = MINGW_COMPILER if compiler.nil?

RESOURCES_DIR = 'resources'.freeze

def fix_csv_app(build_type, compiler)
  if System.os == :linux
    './build/fix_csv/fix_mogasens_csv_app'
  elsif System.os == :windows
    if compiler == MINGW_COMPILER
      'build/fix_csv/fix_mogasens_csv_app.exe'
    elsif compiler == MSVC_COMPILER
      "build/fix_csv/#{build_type}/fix_mogasens_csv_app.exe"
    else
      STDERR.puts("Unsupported compiler: \"#{compiler}\", exiting.")
      exit(1)
    end
  else
    STDERR.puts('Unrecognized operating system, exiting.')
    exit(1)
  end
end

csv_files = Dir["#{RESOURCES_DIR}/**/*.csv"].reject do |file|
  file.include?('_out')
end
csv_files.each do |file|
  puts("Processing #{file}.")
  path = "\"#{File.join(Dir.pwd, file)}\""
  unless system("#{fix_csv_app(build_type, compiler)} #{path}")
    STDERR.puts('Failure running fix_mogasens_csv_app, exiting.')
    exit(1)
  end
  puts('')
end

exit(0)
