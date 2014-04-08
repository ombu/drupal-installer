Drupal Installation Script
==========================
Install OMBU optimized Drupal site in the current directory

Requirements
------------

- In order to install the site after build,
  [butter](https://github.com/ombu/butter) is required, since the site is built
  with `fab drupal.build`. This isn't needed if the `-n` flag is passed to the
  installer.

Usage
-----
drush drupal-installer [OPTION]...

  Options:

    -d [NAME]      Database name. If site is set to install and this option isn't given,
                   then you will be prompted to enter this during the install process.
    -u [USER]      Database user. If site is set to install and this option isn't given,
                   then you will be prompted to enter this during the install process.
    -p             Prompt for database password.
    -s [SITE]      Site name. If not set, then will default to "Site Name"
    -e [EMAIL]     Site email. If not set, then will default to "example@ombuweb.com"
    -m [SHORTNAME] Site shortname. Used to generate paths and filenames. If not set,
                   then will default to the database name.
    -x             Install dev modules and data.
    -n             No install; don't install Drupal site after downloading it
    -h             This help

Example
-------
The following will build Drupal into the current directory using the database 
*database_name*:

> drupalbuild -d database_name -u user -p -s 'Site Name' -e test@test.com -x
