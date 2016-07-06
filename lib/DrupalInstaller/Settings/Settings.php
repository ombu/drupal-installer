<?php

/**
 * @file
 * Holds relevant settings and interpolation code for Installer.
 */

namespace DrupalInstaller\Settings;

use DrupalInstaller\Settings\SettingsException;

class Settings {
  /**
   * DB connection info.
   *
   * @var array
   */
  public $dbInfo;

  /**
   * Site short name.
   *
   * Used in writing default files and directories.
   *
   * @var string.
   */
  public $shortName;

  /**
   * Make file(s) to run.
   *
   * @var array.
   */
  public $makeFiles;

  /**
   * Profile to install.
   *
   * @var string.
   */
  public $profile;

  /**
   * Site settings.
   *
   * @var array
   */
  public $settings;

  /**
   * Path to manifest files.
   *
   * @var string.
   */
  public $manifestPath;

  /**
   * Contructor.
   */
  public function __construct() {
    // The function _drush_sql_get_db_spec() doesn't exist in Drush 7.x+, so
    // have to use alternate means of getting db spec.
    if (function_exists('_drush_sql_get_db_spec')) {
      $this->dbInfo = _drush_sql_get_db_spec();
    }
    else {
      $this->dbInfo = drush_sql_get_class()->db_spec();
    }

    $this->shortName = drush_get_option('short-name', $this->dbInfo['database']);

    $this->settings = array(
      'site_name' => drush_get_option('site-name', 'Site-Install'),
      'site_mail' => drush_get_option('site-email', 'noreply@ombuweb.com'),
    );

    $this->setupManifest();

    $this->profile = drush_get_option('profile', 'default');
    if (!file_exists($this->manifestPath . 'build/profiles/' . $this->profile . '_profile')) {
      throw new SettingsException('Unable to find profile: ' . $file);
    }

    $this->makeFiles = drush_get_option_list('makefiles', array('ombucore'));
    foreach ($this->makeFiles as $file) {
      if (!file_exists($this->manifestPath . $file . '.make')) {
        throw new SettingsException('Unable to find make file: ' . $file);
      }
    }
  }

  /**
   * Interpolate setting settings in a file.
   *
   * By default, all settings within this object will be interpolated according
   * to their defined values. Additional settings can be passed as a second
   * parameter to this method.
   *
   * @param string $file
   *   The file to interpolate.
   * @param array $values
   *   Additional values to interpolate.
   */
  public function interpolate($file, $values = array()) {
    if (drush_get_context('DRUSH_VERBOSE') || drush_get_context('DRUSH_SIMULATE')) {
      drush_log(sprintf("Interpolating values in %s", $file), 'debug');
    }

    if (drush_get_context('DRUSH_SIMULATE')) {
      return TRUE;
    }

    // Add in local settings.
    $values += $values + array(
      '__SHORTNAME__' => $this->shortName,
      '__PROFILENAME__' => $this->profile . '_profile',
      '__THEMENAME__' => $this->shortName . '_theme',
      '__DBNAME__' => $this->dbInfo['database'],
      '__DBUSER__' => $this->dbInfo['username'],
      '__DBPASS__' => $this->dbInfo['password'],
      '__SITENAME__' => $this->settings['site_name'],
      '__SITEMAIL__' => $this->settings['site_mail'],
    );

    $contents = file_get_contents($file);
    $contents = str_replace(array_keys($values), array_values($values), $contents);
    file_put_contents($file, $contents);
  }

  /**
   * Retrieves proper ombucore manifest files from repo.
   */
  protected function setupManifest() {
    // @todo: allow this to be changed to other URLs or even local paths.
    $manifest_repo = 'https://github.com/ombucore/drupal-ombucore.git';

    // Store repo in drush cache so it doesn't have to be cloned every time.
    $cache_path = drush_directory_cache('drupal_installer') . '/ombucore';
    if (!is_dir($cache_path)) {
      drush_shell_exec('git clone %s %s', $manifest_repo, $cache_path);
    }

    // There a bug in git where reset doesn't work when using --git-dir, so the
    // script has to cd into git directory to make any operations.
    $pwd = getcwd();
    chdir($cache_path);

    drush_shell_exec('git fetch origin');
    drush_shell_exec('git fetch --tags origin');

    // Retrieve passed version, defaulting to the tip of origin/master. Try to
    // parse revision both as a remote branch (origin/VERSION) and a tag
    // (VERSION).
    $version = drush_get_option('version', 'master');
    if (!drush_shell_exec('git rev-parse origin/%s', $version) && !drush_shell_exec('git rev-parse %s', $version)) {
      throw new SettingsException('Unknown ombucore version ' . $version);
    }
    $version = drush_shell_exec_output();

    if (!drush_shell_exec('git reset --hard %s', $version[0])) {
      throw new SettingsException('Unable to checkout ombucore version ' . $version);
    }
    chdir($pwd);

    $this->manifestPath = $cache_path . '/';
  }
}
