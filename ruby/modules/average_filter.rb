module AverageFilter
  def self.no_filter
    '--no-moving_average_filter'
  end

  def self.use_filter
    '--moving_average_filter'
  end

  def self.moving_average_filter_option(options)
    if options[:filter_sample_count].nil?
      no_filter
    else
      use_filter
    end
  end

  def self.filter_sample_count_option(options)
    filter_sample_count = options[:filter_sample_count]

    if filter_sample_count.nil?
      '0'
    else
      filter_sample_count.to_s
    end
  end
end
