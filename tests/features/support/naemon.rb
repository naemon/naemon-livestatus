class Naemon
  attr_accessor :brokers
  attr_accessor :config_dir

  def initialize
    @config_dir = (`mktemp --directory --tmpdir=.`).strip()
    @pid = nil
    @configuration =  {
      :command_file => "naemon.cmd",
      :lock_file => "naemon.pid",
      :log_file => "naemon.log",
      :query_socket => "naemon.qh"

    }
    @oconf = nil
    @brokers = {}
  end

  def set_object_config(objectconfig)
    @configuration[:cfg_file] = "objects.cfg" 
    @oconf = objectconfig
  end

  def add_broker(broker)
    @brokers[broker.class.name.downcase.to_sym] = broker
  end

  def wait_for_stop()
    sleep_time = 0.1
    slept = 0
    while slept < 5

      if @pid == nil and @brokers.all? {|_, broker| not broker.is_initialized?}
        return
      end

      sleep(sleep_time)
      slept+=sleep_time
    end
    raise "Error: Naemon, or one of its modules, failed to stop!"
  end

  def wait_for_start()
    sleep_time = 0.1
    slept = 0
    while slept < 5
      if @brokers.all? {|_, broker| broker.is_initialized?}
        return
      end
      sleep(sleep_time)
      slept+=sleep_time
    end
    raise "Error: Naemon, or one of its modules, failed to start!"
  end

  def start()
    naemon_cfg = File.join(@config_dir, "naemon.cfg")
    File.open(naemon_cfg, 'w') { |f|
      @configuration.each do |k, v|
        f.write("#{k.to_s}=#{v}\n")
      end

      @brokers.each do |_, broker|
        f.write("broker_module=#{broker.broker_config}")
      end
    }

    if @configuration[:cfg_file]
      File.open(File.join(@config_dir, @configuration[:cfg_file]), 'w') { |f|
        f.write(@oconf.configfile)
      }
    end
    `naemon --allow-root -d #{@config_dir}/naemon.cfg`
    self.wait_for_start
  end

  def pid()
    begin
      File.read(File.join(@config_dir, "naemon.pid")).to_i
    rescue Errno::ENOENT
      nil
    end
  end

  def stop()
    `kill #{self.pid}`
    wait_for_stop
  end
end

class NaemonModule
  def broker_config
    raise "Undefined NaemonModule"
  end

  def is_initialized?()
    true
  end
end

class Livestatus < NaemonModule
  attr_accessor :last_response, :clobber_data

  def initialize(sockpath)
    @sockpath = sockpath
  end

  def broker_config
    if ENV['CUKE_LIVESTATUS_PATH']
      so_path = ENV['CUKE_LIVESTATUS_PATH']
    else
      so_path = "/usr/lib64/naemon-livestatus/livestatus.so"
    end
    "#{so_path} #{@sockpath} debug=1"
  end

  def query(q)
    if ENV['CUKE_UNIXCAT_PATH']
      unixcat = ENV['CUKE_UNIXCAT_PATH']
    else
      unixcat = "unixcat"
    end
    cmd = "/bin/echo -e \"#{q}\"| #{unixcat} #{@sockpath}"
    @last_response = `#{cmd}`.split("\n")
  end
  
  def is_initialized?()
    File.exists? @sockpath
  end

  def clobber(n_queries, idle_time)
    if ENV['CUKE_CLOBBER_PATH']
      clobber = ENV['CUKE_CLOBBER_PATH']
    else
      clobber = 'ls-clobber'
    end
    clobber_output = `#{clobber} #{@sockpath} #{n_queries} #{idle_time}`
    @clobber_data = JSON.load(clobber_output)
  end
end
