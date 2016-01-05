# Drupal Services Demo

A Vagrant Machine to Run Drupal Services Demo

- Debian Based machine
- Shared folders via NFS
- Default IP : 10.11.12.201
- Default Shared : /var/www

Instalation

Access with vagrant ssh and make sudo -s

Use root as password of mysql so the drupal setting will work without changes.

```
sudo -s
apt-get update
apt-get install phpmyadmin mysql-server memcached php5-memcached php5-xdebug php-apc php5-dev build-essential php-pear vim-nox drush
cd /var/www
rm -rf html
drush make drupal.make html
chmod -R 0777 html
cd html
drush si --account-mail=your@mail.com --account-name=root --account-pass=root --db-su=root --db-su-pw=root --db-url="mysql://root:root@127.0.0.1/drupal"
```

Edit the default host

```
vim /etc/apache2/sites-enabled/000-default.conf
```

Add these to the vhost.

```
<Directory /var/www/html>
    AllowOverride All
</Directory>
```

To make it work the rewrite rules.

```
a2enmod rewrite
service apache2 restart
```

Now go with your browser to

http://10.11.12.201/user






Install mysql database located into db/drupal.sql

Access to http://10.11.12.201

Drupal User is root, pass is root
