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

puts("confusion_matrix.rb: Starting.\n"\
     "build_type is \"#{build_type}\".\n"\
     "compiler is \"#{compiler}\".")

def confusion_matrix_app(the_build_type, the_compiler)
  if System.os == :linux
    './build/confusion_matrix/confusion_matrix_app'
  elsif System.os == :windows
    if the_compiler == MINGW_COMPILER
      'build\\confusion_matrix\\confusion_matrix_app.exe'
    elsif the_compiler == MSVC_COMPILER
      "build\\confusion_matrix\\#{the_build_type}\\confusion_matrix_app.exe"
    else
      STDERR.puts("Unsupported compiler: \"#{the_compiler}\", exiting.")
      exit(1)
    end
  else
    STDERR.puts('Unrecognized operating system, exiting.')
    exit(1)
  end
end

run_string = confusion_matrix_app(build_type, compiler)
unless system(run_string)
  STDERR.puts("Failure running \"#{run_string}\", exiting.")
  exit(1)
end

exit(0)
