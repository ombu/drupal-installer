Drupal Installation Script
==========================
Install OMBU optimized Drupal site in [DIRECTORY] or the current directory if
no directory is given.

Usage
-----
drupalbuild [OPTION]... [DIRECTORY]

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
The following will build Drupal into the drupal/ directory using the database 
*database_name*:

> drupalbuild -d database_name -u user -p -s 'Site Name' -e test@test.com -x drupal
