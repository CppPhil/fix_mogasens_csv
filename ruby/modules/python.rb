require_relative 'system'

module Python
  def self.interpreter
    if System.os == :linux
      'python3'
    elsif System.os == :windows
      'python.exe'
    else
      STDERR.puts('Unsupported operating system, exiting.')
      exit(1)
    end
  end
end
