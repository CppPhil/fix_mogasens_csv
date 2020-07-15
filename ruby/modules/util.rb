module Util
  def self.build_type(options)
    result = options[:build_type]
    debug_build_type = 'Debug'.freeze
    release_build_type = 'Release'.freeze
    result = release_build_type if result.nil?

    if result != debug_build_type && result != release_build_type
      STDERR.puts("Invalid build type \"#{result}\"! "\
              "Allowable values are \"#{debug_build_type}\" "\
              "and \"#{release_build_type}\".")
      exit(1)
    end

    result
  end
end
