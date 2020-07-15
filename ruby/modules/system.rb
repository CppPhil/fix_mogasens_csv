module System
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
end
