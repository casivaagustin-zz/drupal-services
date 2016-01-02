# -*- mode: ruby -*-
# vi: set ft=ruby :
# vi: set tabstop=2 shiftwidth=2 expandtab

boxes = [
  { 
    :name => "drupal-services", 
    :ip => [
      "10.11.12.201"
    ],
    :synced_folders => [
      {'src' => '/var/www'},
    ], 
  },
]

Vagrant.configure("2") do |config|
  boxes.each do |opts|
    config.vm.define opts[:name] do |config|
      config.vm.box = "ARTACK/debian-jessie"
      config.vm.box_url = "https://atlas.hashicorp.com/ARTACK/boxes/debian-jessie" 
      config.vm.boot_timeout = 600

      config.vm.network :public_network

      if opts[:ip].is_a? Array
        opts[:ip].each do |ip|
          config.vm.network :private_network, ip: ip
        end
      else
        config.vm.network :private_network, ip: opts[:ip]
      end

      opts[:synced_folders].each do |hash|
        hash.each do |folder1, folder2|
          config.vm.synced_folder folder1, folder2, :nfs => true, :mount_options => ['nolock,vers=3,udp,noatime']
        end
      end

      config.vm.provider :virtualbox do |vb|
        vb.customize ["modifyvm", :id, "--cpuexecutioncap", "50"]
        vb.customize ["modifyvm", :id, "--ioapic", "on"]
        vb.customize ["modifyvm", :id, "--memory", "2048"]
        vb.customize ["modifyvm", :id, "--cpus", "1"]   
      end

    end
  end
end
