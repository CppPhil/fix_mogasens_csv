require 'rbconfig'
require 'etc'

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

def dev_null
  if os == :linux
    '/dev/null'
  elsif os == :windows
    'NUL'
  else
    STDERR.puts('Unsupported operating system, exiting.')
    exit(1)
  end
end

working_directory = Dir.pwd
plotter = "#{working_directory}/mogasens_plotter/main.py"

def python_interpreter
  if os == :linux
    'python3'
  elsif os == :windows
    'python.exe'
  else
    STDERR.puts('Unsupported operating system, exiting.')
    exit(1)
  end
end

Dir.chdir('resources')

csv_files = Dir['./**/*.csv'].select do |file|
  file.end_with?('_out.csv')
end

other_csv_files = Dir['./**/*.csv'].reject { |file| file.end_with?('_out.csv') }

if csv_files.size != other_csv_files.size
  STDERR.puts('Unexpected amount of _out.csv files, exiting.')
  exit(1)
end

sensors = [
  769, # right arm
  770, # belly
  771, # chest
  772  # left arm
]

imus = %w[accelerometer gyroscope]

proc_count = Etc.nprocessors
total_count = csv_files.size * sensors.size * imus.size
counter = 1
threads = []

puts("Generating images with #{proc_count} threads.")

csv_files.each do |csv_file|
  sensors.each do |sensor|
    imus.each do |imu|
      threads << Thread.new do
        unless system("#{python_interpreter} #{plotter} #{csv_file} #{sensor} "\
                      "#{imu} > #{dev_null}")
          STDERR.puts("\"#{python_interpreter} #{plotter} #{csv_file}"\
                      " #{sensor} #{imu}\" failed, exiting.")
          exit(1)
        end
      end

      if (counter % proc_count).zero?
        threads.each(&:join)
        threads.clear
      end

      printf("%.2f%%\r", counter.to_f / total_count.to_f * 100.0)
      $stdout.flush
      counter += 1
    end
  end
end

threads.each(&:join)
puts('100.00%')
$stdout.flush

puts('Done.')

exit(0)
