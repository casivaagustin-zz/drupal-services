# Drupal Services Demo

A Vagrant Machine to Run Drupal Services Demo

- Debian Based machine
- Shared folders via NFS
- Default IP : 10.11.12.201
- Default Shared : /var/www

This command install all that I usually need for PHP
   
```
apt-get update
apt-get install phpmyadmin mysql-server memcached php5-memcached php5-xdebug php-apc php5-dev build-essential php-pear vim-nox
apt-get install drush
```

Use root as password of mysql so the drupal setting will work without changes

Install mysql database located into db/drupal.sql

Access to http://10.11.12.201

Drupal User is root, pass is root
