<?php

/**
 * @file
 * Drush command for quickly spinning up an OMBU default site.
 */

/**
 * Implements hook_drush_command().
 */
function drupal_installer_drush_command() {
  $items = array();

  $items['drupal-installer'] = array(
    'description' => 'Setup and install OMBU optimized Drupal site the current directory',
    'callback' => 'drupal_installer_drush_callback',
    'options' => array(
      'db-url' => array(
        'description' => 'A Drupal 6 style database URL. Only required for initial install - not re-install.',
        'example-value' => 'mysql://root:pass@host/db',
        'required' => TRUE,
      ),
      'short-name' => array(
        'description' => 'The short name for the project. Will be used to name theme, profile, and base feature. If not set then the database name will be used. Only letter, numbers and underscores are allowed.',
        'example-value' => 'my_site',
      ),
      'site-name' => 'Defaults to Site-Install',
      'site-mail' => 'From: for system mailings. Defaults to noreply@ombuweb.com',
      'demo' => 'Install a demo tiles site',
    ),
    'aliases' => array('di'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  return $items;
}

/**
 * Drush callback for installer.
 */
function drupal_installer_drush_callback() {
  $command = drush_get_command();

  // Get options.
  $db_spec = _drush_sql_get_db_spec();
  $short_name = drush_get_option('short-name', $db_spec['database']);
  $site_name = drush_get_option('site-name', 'Site-Install');
  $site_mail = drush_get_option('site-email', 'noreply@ombuweb.com');

  // Check if build directory exists
  if (file_exists('public')) {
    drush_set_error(dt("Aborting: public directory already exists, delete first before running this command"));
    return FALSE;
  }

  // Build drupal site with make file, keeping repo directories
  $make_file = $command['path'] . '/includes/ombu7.make';
  $return = drush_invoke_process('@none', 'make', array(
    $make_file,
    'public'
  ), array(
    '--no-gitinfofile',
    '--working-copy',
  ));
  if (!$return || $return['error_status']) {
    drush_set_error(dt('Error building make file'));
    return FALSE;
  }

  // Settings to replace in included files.
  $settings = array(
    '__SHORTNAME__' => $short_name,
    '__DBNAME__' => $db_spec['database'],
    '__DBUSER__' => $db_spec['username'],
    '__DBPASS__' => $db_spec['password'],
    '__SITENAME__' => $site_name,
    '__SITEMAIL__' => $site_mail,
  );

  // Setup default feature.
  if (drush_get_option('demo')) {
    $feature_name = 'tiles_base';
  }
  else {
    $feature_name = $short_name . '_base';
  }
  drush_op('mkdir', 'public/sites/all/modules/features');
  $feature_path = 'public/sites/all/modules/features/' . $feature_name;
  drush_shell_exec('cp -r %s %s', $command['path'] . '/includes/features/base', $feature_path);
  foreach (glob($feature_path . '/*') as $filename) {
    $filename = basename($filename);
    $destination = $feature_path . '/' . str_replace('base', $feature_name, $filename);
    drush_op('rename', $feature_path . '/' . $filename, $destination);
    drupal_installer_interpolate_values($destination, $settings + array('base' => $feature_name));
  }
  drush_log(dt('Feature setup'), 'ok');

  // Setup theme.
  $theme_name = $short_name . '_theme';
  $theme_path = 'public/sites/all/themes/' . $theme_name;

  drush_op('mkdir', $theme_path);

  $info_file = <<<EOD
name = $site_name Theme
description = $site_name Theme
base theme = boots_grid
package = OMBU
version = 7.x-0.1
core = 7.x

stylesheets[all][] = css/style.css

settings[tiles_regions][content_top] = Content Top
settings[tiles_regions][content] = Content
settings[tiles_regions][content_bottom] = Content Bottom
EOD;
  drush_op('file_put_contents', $theme_path . '/' . $theme_name . '.info', $info_file);

  drush_op('mkdir', $theme_path . '/css');
  drush_op('touch', $theme_path . '/css/style.css');
  drush_op('touch', $theme_path . '/css/style.less');

  drush_log(dt('Theme setup'), 'ok');

  // Setup profile. If spinning up a demo site, load the tiles profile (which
  // sets up default blocks).
  if (drush_get_option('demo')) {
    $profile_name = 'tiles_profile';
    $settings['__PROFILENAME__'] = $profile_name;

    drush_shell_exec('cp -r %s %s', $command['path'] . '/includes/profiles/tiles_profile', 'public/profiles/tiles_profile');
  }
  else {
    $profile_name = $short_name . '_profile';
    $settings['__PROFILENAME__'] = $profile_name;
    $profile_path = 'public/profiles/' . $profile_name;

    drush_shell_exec('cp -r %s %s', $command['path'] . '/includes/profiles/default_profile', $profile_path);

    $profile_files = array(
      'default_profile.info' => $profile_name . '.info',
      'default_profile.install' => $profile_name . '.install',
      'default_profile.profile' => $profile_name . '.profile',
    );
    foreach ($profile_files as $source => $destination) {
      drush_op('rename', $profile_path . '/' . $source, $profile_path . '/' . $destination);
      drupal_installer_interpolate_values($profile_path . '/' . $destination, $settings + array('default_profile' => $profile_name));
    }

    drush_op('mkdir', $profile_path . '/config');

    $theme_file = <<<EOD
add:
  default_theme: $theme_name
EOD;
    drush_op('file_put_contents', $profile_path . '/config/theme.yml', $theme_file);

    $module_file = <<<EOD
add:
  - $feature_name
EOD;
    drush_op('file_put_contents', $profile_path . '/config/modules.yml', $module_file);

  }
  drush_log(dt('Profile setup'), 'ok');

  // Copy default included files.
  $files = array(
    'includes/fabfile.py'                        => 'fabfile.py',
    'includes/README.md'                         => 'README.md',
    'includes/settings/settings.development.php' => 'public/sites/default/settings.development.php',
    'includes/settings/settings.qa.php'          => 'public/sites/default/settings.qa.php',
    'includes/settings/settings.staging.php'     => 'public/sites/default/settings.staging.php',
    'includes/settings/settings.production.php'  => 'public/sites/default/settings.production.php',
    'includes/settings/settings.production.php'  => 'public/sites/default/settings.production.php',
  );
  foreach ($files as $source => $destination) {
    drush_op('copy', $command['path'] . '/' . $source, $destination);
    drupal_installer_interpolate_values($destination, $settings);
  }
  drush_op('copy', 'public/sites/default/settings.development.php', 'public/sites/default/settings.php');
  drush_op('copy', 'public/sites/all/modules/custom/ombudashboard/ombudashboard.admin_blocks.inc.example', 'public/sites/default/ombudashboard.admin_blocks.inc');

  // Setup git.
  drush_log(dt('Initializing git repo'), 'ok');
  drush_shell_exec('git init');

  // .gitignore file.
  $gitignore = <<<EOD
sites/default/settings.php
sites/default/files
EOD;
  drush_op('file_put_contents', '.gitignore', $gitignore);

  drush_log(dt('Setting up submodules'));

  // Parse makefile to get correct branches for each submodule.
  $info = make_parse_info_file($make_file);
  drush_shell_exec('git submodule add git@github.com:ombu/drupal-ombucore.git public/sites/all/modules/custom/ombucore');
  drush_shell_exec('git submodule add git@github.com:ombu/drupal-ombucleanup.git public/sites/all/modules/custom/ombucleanup');
  drush_shell_exec('git submodule add git@github.com:ombu/drupal-ombudashboard.git public/sites/all/modules/custom/ombudashboard');
  drush_shell_exec('git submodule add git@github.com:ombu/drupal-ombuseo.git public/sites/all/modules/custom/ombuseo');
  drush_shell_exec('git submodule add git@github.com:ombu/drupal-ombubeans.git public/sites/all/modules/custom/ombubeans');
  drush_shell_exec('git submodule add git@github.com:ombu/drupal-beancontainer.git public/sites/all/modules/custom/beancontainer');
  drush_shell_exec('git submodule add git@github.com:ombu/drupal-tiles.git public/sites/all/modules/custom/tiles');
  drush_shell_exec('git submodule add git@github.com:ombu/drupal-ombuslide.git public/sites/all/modules/custom/ombuslide');
  drush_shell_exec('git submodule add git@github.com:ombu/drupal-boots.git public/sites/all/themes/boots');

  drush_shell_exec('git add public fabfile.py README.md .gitignore');
  drush_shell_exec('git add -f public/sites/default/settings.*');
  drush_shell_exec('git commit -m "Initial install of OMBU Core"');

  // Build site.
  $command = 'fab drupal.build';
  proc_close(proc_open($command, array(0 => STDIN, 1 => STDOUT, 2 => STDERR), $pipes));

  drush_log(dt('OMBU optimized Drupal site successfully setup. You are ready to roll'), 'ok');
}

/**
 * Helper function to interpolate values in a file.
 */
function drupal_installer_interpolate_values($file, $values) {
  if (drush_get_context('DRUSH_VERBOSE') || drush_get_context('DRUSH_SIMULATE')) {
    drush_log(sprintf("Interpolating values in  %s", $file), 'debug');
  }

  if (drush_get_context('DRUSH_SIMULATE')) {
    return TRUE;
  }

  $contents = file_get_contents($file);
  $contents = str_replace(array_keys($values), array_values($values), $contents);
  file_put_contents($file, $contents);
}
