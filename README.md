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

    --db-url=<mysql://root:pass@host/db>      A Drupal 6 style database URL. Required.
    --demo                                    Install a demo site
    --makefiles=<ombucore,publishing>         What Drush makefiles to run prior
                                              to installing Drupal. Defaults to
                                              "ombucore" but if specified,
                                              "ombucore" makefile will not be
                                              run.
    --profile=<ombudemo>                      What profile to install.
                                              Defaults to "default"
    --short-name=<my_site>                    The short name for the project.
                                              Will be used to name theme,
                                              profile, and base feature. If not
                                              set then the database name will be
                                              used. Only letter, numbers and
                                              underscores are allowed.
    --site-mail                               From: for system mailings.
                                              Defaults to noreply@ombuweb.com
    --site-name                               Defaults to Site-Install
    --version=<0.6>                           A valid reference for the ombucore
                                              repo, i.e. tag or branch. Defaults
                                              to "origin/master"

Example
-------
The following will build Drupal into the current directory using the database 
*database_name*:

> drush drupal-installer --db-url=mysql://root:pass@localhost/test --site-name="Test Site" --version=release/0.8 --makefiles=ombucore,publishing
