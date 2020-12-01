require 'rbconfig'
require_relative 'modules/system'
require_relative 'modules/util'
require_relative 'modules/command_line'

MINGW_COMPILER = 'MinGW'.freeze
MSVC_COMPILER = 'MSVC'.freeze

options = CommandLine.parse([CommandLine.compare_segmentation_mode_option, \
                             CommandLine.build_type_option, \
                             CommandLine.compiler_option])

mode = options[:mode]
mode = 'AllDataSets' if mode.nil?

build_type = Util.build_type(options)

compiler = options[:compiler]
compiler = MINGW_COMPILER if compiler.nil?

puts("compare_segmentation.rb: Starting.\n"\
     "mode is \"#{mode}\".\n"\
     "build_type is \"#{build_type}\".\n"\
     "compiler is \"#{compiler}\".")

RESOURCES_DIR = 'resources'.freeze

def compare_segmentation_app(the_build_type, the_compiler)
  if System.os == :linux
    './build/compare_segmentation/compare_segmentation_app'
  elsif System.os == :windows
    if the_compiler == MINGW_COMPILER
      'build\\compare_segmentation\\compare_segmentation_app.exe'
    elsif the_compiler == MSVC_COMPILER
      "build\\compare_segmentation\\#{the_build_type}\\compare_segmentation_app.exe"
    else
      STDERR.puts("Unsupported compiler: \"#{the_compiler}\", exiting.")
      exit(1)
    end
  else
    STDERR.puts('Unrecognized operating system, exiting.')
    exit(1)
  end
end

out_file = 'segmentation_comparison/out.txt'
puts("Redirecting stdout to \"#{out_file}\".")
run_string = "#{compare_segmentation_app(build_type, compiler)} #{mode} > #{out_file}"
unless system(run_string)
  STDERR.puts("Failure running \"#{run_string}\", exiting.")
  exit(1)
end

exit(0)
