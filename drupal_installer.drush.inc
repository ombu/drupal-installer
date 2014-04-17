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
      'version' => array(
        'description' => 'A valid reference for the ombucore repo, i.e. tag or branch. Defaults to "origin/master"',
        'example-value' => '0.6',
      ),
      'profile' => array(
        'description' => 'What profile to install. Defaults to "default"',
        'example-value' => 'ombudemo',
      ),
      'makefiles' => array(
        'description' => 'What Drush makefiles to run prior to installing Drupal. Defaults to "ombucore" but if specified, "ombucore" makefile will not be run.',
        'example-value' => 'ombucore,publishing',
      ),
      'demo' => 'Install a demo site',
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
  // Setup autoloader.
  drupal_installer_autoloader();

  // Retrieve settings and installer.
  try {
    $settings = new \DrupalInstaller\Settings\Settings();
    $installer = new \DrupalInstaller\Installer\Installer($settings);

    // Verify requirements.
    $installer->verify();

    // Run build.
    $installer->build();
  }
  catch (\DrupalInstaller\Settings\SettingsException $e) {
    drush_set_error('Settings error: ' . $e->getMessage());
    return FALSE;
  }
  catch (\DrupalInstaller\Installer\InstallerException $e) {
    drush_set_error('Install error: ' . $e->getMessage());
    return FALSE;
  }
}

/**
 * Sets up autoloading for installer.
 *
 * @return \Composer\Autoload\ClassLoader
 *   Composer autoloader.
 */
function drupal_installer_autoloader() {
  $loader = require 'vendor/autoload.php';
  return $loader;
}
