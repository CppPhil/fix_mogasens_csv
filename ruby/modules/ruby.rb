require_relative 'system'

module Ruby
  def self.interpreter
    if System.os == :linux
      'ruby'
    elsif System.os == :windows
      'external\\goto-ruby\\windows_data\\ruby\\rubyinstaller-2.6.3-1-x86\\'\
      'bin\\ruby.exe'
    else
      STDERR.puts('Unsupported operating system, exiting.')
      exit(1)
    end
  end
end
