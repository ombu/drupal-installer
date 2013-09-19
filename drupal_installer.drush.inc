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
      ),
      'short_name' => array(
        'description' => 'The short name for the project. Will be used to name theme, profile, and base feature. If not set then the database name will be used. Only letter, numbers and underscores are allowed.',
        'example-value' => 'my_site',
      ),
      'site-name' => 'Defaults to Site-Install',
      'site-mail' => 'From: for system mailings. Defaults to admin@example.com',
      'dev' => 'Install dev modules and data',
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

  // Check if build directory exists
  if (file_exists('public')) {
    drush_set_error(dt("Aborting: public directory already exists, delete first before running this command"));
    return;
  }

  // Build drupal site with make file, keeping repo directories
  drush_invoke_process('@none', 'make', array(
    $command['path'] . '/include/ombu7.make',
    'public'
  ), array(
    '--no-gitinfofile',
  ));

  // Setting to interpolate in included files.
  $settings = array(
    '_DBNAME__',
    '_DBUSER__',
    '_DBPASS__',
    '_SITENAME__',
    '_SITEMAIL__',
  );

  // Copy default included files.
  $files = array(
    'include/fabfile.py' => 'fabfile.py',
    'include/README.md' => 'README.md',
    'settings.development.php' => 'sites/default/settings.development.php',
    'settings.qa.php' => 'sites/default/settings.qa.php',
    'settings.staging.php' => 'sites/default/settings.staging.php',
    'settings.production.php' => 'sites/default/settings.production.php',
  );
  foreach ($files as $source => $destination) {
    drush_op('copy', $command['path'] . '/' . $source, $destination);
    drupal_installer_iterpolate_values($destination, $settings);
  }

  // Setup ombudashboard.
  drush_op('copy', 'public/sites/all/modules/custom/ombudashboard/ombudashboard.admin_blocks.inc.example', 'public/sites/default/ombudashboard.admin_blocks.inc');

  // Setup theme.
  drush_log(dt('Seting up theme'));

  $theme_name = 'shortname'; //$shortname;
  $theme_path = 'public/sites/all/themes/' . $theme_name;

  drush_exec('cp -r %s %s', 'public/sites/all/themes/boots/boots_grid', $theme_path);
  drush_op('rename', 'public/sites/all/themes/' . $theme_name . '/boots_grid.info', $theme_path . '/' . $theme_name . '.info');

  $replacements = array(
    'name =.*'        => 'name = ' . $sitename . ' Theme',
    'description =.*' => 'description = ' . $sitename . ' Theme',
    'base theme =.*'  => 'base theme = boots_grid',
  );
  drupal_installer_interpolate_values($theme_path . '/' . $theme_name . '.info', $replacements);

  drupal_installer_interpolate_values($theme_path . '/template.php', array('boots_grid' => $theme_name));
}

/**
 * Helper function to interpolate values in a file.
 */
function drupal_installer_interpolate_values($file, $values) {
  $contents = file_get_contents($file);
  $contents = str_replace(array_keys($values), array_values($values), $contents);
  file_put_contents($file, $contents);
}
