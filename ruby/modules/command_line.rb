require 'optparse'

module CommandLine
  BUILD_TYPE_OPTION = 'build_type'.freeze
  COMPILER_OPTION = 'compiler'.freeze
  HELP_OPTION = 'help'.freeze

  def parse(opts)
    options = {}

    OptionParser.new do |opt|
      if opts.include?(BUILD_TYPE_OPTION)
        opt.on('--build_type=BUILD_TYPE', 'Debug | Release') do |o|
          options[:build_type] = o
        end
      end

      mingw_compiler = 'MinGW'.freeze
      msvc_compiler = 'MSVC'.freeze
      compilers = "#{mingw_compiler} | #{msvc_compiler}".freeze

      if opts.include?(COMPILER_OPTION)
        opt.on('--compiler=COMPILER', compilers) do |o|
          options[:compiler] = o
        end
      end

      if opts.include?(HELP_OPTION)
        opt.on_tail('-h', '--help', 'Show this message') do
          STDERR.puts opt
          exit(0)
        end
      end
    end.parse!

    options
  end
end
