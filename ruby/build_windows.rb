require 'optparse'
require 'etc'

MINGW_COMPILER = 'MinGW'.freeze
MSVC_COMPILER = 'MSVC'.freeze

def cmake_generator(compiler)
  if compiler == MINGW_COMPILER
    '"MinGW Makefiles"'
  elsif compiler == MSVC_COMPILER
    '"Visual Studio 16 2019" -A x64'
  else
    STDERR.puts("Compiler \"#{compiler}\" is not supported, exiting.")
    exit(1)
  end
end

options = {}

OptionParser.new do |opt|
  opt.on('--build_type=BUILD_TYPE', 'Debug | Release') do |o|
    options[:build_type] = o
  end
  opt.on('--compiler=COMPILER', "#{MINGW_COMPILER} | #{MSVC_COMPILER}") do |o|
    options[:compiler] = o
  end
  opt.on_tail('-h', '--help', 'Show this message') do
    STDERR.puts opt.to_s.gsub('build_windows', 'build.bat')
    exit(1)
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

MINGW_DIR = 'mingw_dist'.freeze

if compiler == MINGW_COMPILER
  Dir.mkdir(MINGW_DIR) unless Dir.exist?(MINGW_DIR)
  Dir.chdir(MINGW_DIR)

  NUWEN_ARCHIVE = 'mingw-17.1-without-git.exe'.freeze
  URL = "https://nuwen.net/files/mingw/#{NUWEN_ARCHIVE}".freeze

  if Dir.exist?('MinGW')
    puts("#{MINGW_DIR}/MinGW exists, assuming it contains "\
         'a valid MinGW distribution!')
  else
    unless system('bitsadmin.exe /transfer "MinGW Download" '\
                  "\"#{URL}\" \"%cd%\\#{NUWEN_ARCHIVE}\"")
      STDERR.puts("Failed to download MinGW from #{URL}, exiting.")
      exit(1)
    end

    unless system("#{NUWEN_ARCHIVE} -y")
      STDERR.puts("Couldn't extract \"#{NUWEN_ARCHIVE}\", exiting.")
      exit(1)
    end
  end

  Dir.chdir('..')
end

BUILD_DIRECTORY = 'build'.freeze
Dir.mkdir(BUILD_DIRECTORY) unless Dir.exist?(BUILD_DIRECTORY)
Dir.chdir(BUILD_DIRECTORY)

LOAD_DISTRO_PATHS = "cd ..\\#{MINGW_DIR}\\MinGW && set_distro_paths.bat && "\
                    'cd ..\\..\\build'.freeze

cmake_command = nil

if compiler == MINGW_COMPILER
  cmake_command = "#{LOAD_DISTRO_PATHS} && "\
                  "cmake -G #{cmake_generator(compiler)} .."
elsif compiler == MSVC_COMPILER
  cmake_command = "cmake -G #{cmake_generator(compiler)} .."
else
  STDERR.puts("Invalid compiler: \"#{compiler}\"! Exiting.")
  exit(1)
end

unless system(cmake_command)
  STDERR.puts('Failure running CMake.')
  STDERR.puts('Is CMake on the PATH?')
  STDERR.puts('If CMake is not installed use the installer '\
              'from https://github.com/Kitware/CMake/releases/download/v3.18.0-rc4/cmake-3.18.0-rc4-win64-x64.msi')
  STDERR.puts('Exiting.')
  exit(1)
end

if compiler == MINGW_COMPILER
  unless system("#{LOAD_DISTRO_PATHS} && make -j#{Etc.nprocessors}")
    STDERR.puts('make failed. MinGW build aborted, exiting.')
    exit(1)
  end
elsif compiler == MSVC_COMPILER
  unless system('MSBuild.exe mogasens_csv.sln '\
                "-property:Configuration=#{build_type} /maxcpucount")
    STDERR.puts('Failure running MSBuild.')
    STDERR.puts('Are you running the "Developer Command Prompt for VS2019"?')
    STDERR.puts('Exiting.')
    exit(1)
  end
else
  STDERR.puts("Unsupported compiler: #{compiler}.")
  STDERR.puts('MinGW-w64 and MSVC19 are supported.')
  STDERR.puts('Exiting.')
  exit(1)
end

exit(0)
