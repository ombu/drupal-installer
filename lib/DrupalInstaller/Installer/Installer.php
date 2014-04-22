<?php

/**
 * @file
 * Installer script.
 */

namespace DrupalInstaller\Installer;

use DrupalInstaller\Settings\Settings;
use DrupalInstaller\Installer\InstallerException;

class Installer {
  /**
   * Settings object.
   *
   * @var Settings
   */
  protected $settings;

  /**
   * Constructor
   *
   * @param Settings $settings
   *   A fully loaded settings object.
   */
  public function __construct(Settings $settings) {
    $this->settings = $settings;
  }

  /**
   * Verify build requirements.
   *
   * @throws InstallerException
   */
  public function verify() {
    if (file_exists('public')) {
      throw new InstallerException('Aborting: public directory already exists, delete first before running this command');
    }
  }

  /**
   * General build task.
   */
  public function build() {
    $this->buildMake();
    $this->buildProfile();
    $this->buildFeature();
    $this->buildTheme();

    $this->setupIncludes();
    $this->setupGit();

    $command = 'fab drupal.build';
    proc_close(proc_open($command, array(0 => STDIN, 1 => STDOUT, 2 => STDERR), $pipes));
  }

  /**
   * Build a make file.
   */
  protected function buildMake() {

    foreach ($this->settings->makeFiles as $file) {
      $make_file = $this->settings->manifestPath . $file . '.make';

      $options = array(
        '--no-gitinfofile',
        '--working-copy',
      );

      $info = make_parse_info_file($make_file);
      $make_path = 'public';
      $cd_path = NULL;
      if (!empty($info['no_core'])) {
        $options[] = '--no-core';
        $options[] = '-y';
        $make_path = NULL;

        // drush make has to be run in the Drupal install directory if there's
        // no core specified.
        $cd_path = TRUE;
        chdir('public');
      }

      $return = drush_invoke_process('@none', 'make', array(
        $make_file,
        $make_path
      ), $options);

      if (!$return || $return['error_status']) {
        throw new InstallerException('Error building make file');
      }

      // Return to parent directory if installing a make file without core.
      if ($cd_path) {
        chdir('..');
      }
    }
  }

  /**
   * Setup profile for site.
   */
  protected function buildProfile() {
    $values = array();

    $profile_name = $this->settings->profile . '_profile';

    // Setup profile. If building a profile other than default, no name
    // alterations need to happen.
    if ($this->settings->profile != 'default') {
      $profile_path = 'public/profiles/' . $profile_name;
      drush_shell_exec('cp -r %s %s', $this->settings->manifestPath . '/build/profiles/' . $profile_name, $profile_path);
      $this->settings->interpolate($profile_path . '/config/modules.yml');
    }
    else {
      $profile_name = $this->settings->shortName . '_profile';
      $values['__PROFILENAME__'] = $profile_name;
      $profile_path = 'public/profiles/' . $profile_name;

      drush_shell_exec('cp -r %s %s', $this->settings->manifestPath . '/build/profiles/default_profile', $profile_path);

      $profile_files = array(
        'default_profile.info' => $profile_name . '.info',
        'default_profile.install' => $profile_name . '.install',
        'default_profile.profile' => $profile_name . '.profile',
      );
      foreach ($profile_files as $source => $destination) {
        drush_op('rename', $profile_path . '/' . $source, $profile_path . '/' . $destination);
        $this->settings->interpolate($profile_path . '/' . $destination, $values + array('default_profile' => $profile_name));
      }

      drush_op('mkdir', $profile_path . '/config');

      $theme_file = <<<EOD
add:
  default_theme: $theme_name
EOD;
      drush_op('file_put_contents', $profile_path . '/config/theme.yml', $theme_file);

      $feature_name = $this->settings->shortName . '_base';
      $module_file = <<<EOD
add:
  - $feature_name
EOD;
      drush_op('file_put_contents', $profile_path . '/config/modules.yml', $module_file);

    }
    drush_log(dt('Profile setup'), 'ok');
  }

  /**
   * Setup base feature.
   */
  protected function buildFeature() {
    $feature_name = $this->settings->shortName . '_base';
    $feature_path = 'public/sites/all/modules/features/';
    drush_op('mkdir', $feature_path);

    $feature_path .= $feature_name;
    drush_shell_exec('cp -r %s %s', $this->settings->manifestPath . '/build/features/base', $feature_path);

    foreach (glob($feature_path . '/*') as $filename) {
      $filename = basename($filename);
      $destination = $feature_path . '/' . str_replace('base', $feature_name, $filename);
      drush_op('rename', $feature_path . '/' . $filename, $destination);
      $this->settings->interpolate($destination, array('base' => $feature_name));
    }

    drush_log(dt('Feature setup'), 'ok');
  }

  /**
   * Setup base theme.
   */
  protected function buildTheme() {
    $theme_name = $this->settings->shortName . '_theme';
    $theme_path = 'public/sites/all/themes/' . $theme_name;

    drush_op('mkdir', $theme_path);

    $info_file = $theme_path . '/' . $theme_name . '.info';
    drush_shell_exec('cp -r %s %s', $this->settings->manifestPath . '/build/theme/default_theme.info', $info_file);
    $this->settings->interpolate($info_file);

    drush_op('mkdir', $theme_path . '/css');
    drush_op('touch', $theme_path . '/css/style.css');
    drush_op('touch', $theme_path . '/css/style.less');

    drush_log(dt('Theme setup'), 'ok');
  }

  /**
   * Setup include files.
   */
  protected function setupIncludes() {
    // Copy default included files.
    $files = array(
      'build/fabfile.py'                        => 'fabfile.py',
      'build/README.md'                         => 'README.md',
      'build/settings/settings.development.php' => 'public/sites/default/settings.development.php',
      'build/settings/settings.qa.php'          => 'public/sites/default/settings.qa.php',
      'build/settings/settings.staging.php'     => 'public/sites/default/settings.staging.php',
      'build/settings/settings.production.php'  => 'public/sites/default/settings.production.php',
      'build/settings/settings.production.php'  => 'public/sites/default/settings.production.php',
    );
    foreach ($files as $source => $destination) {
      drush_op('copy', $this->settings->manifestPath . '/' . $source, $destination);
      $this->settings->interpolate($destination);
    }
    drush_op('copy', 'public/sites/default/settings.development.php', 'public/sites/default/settings.php');
    drush_op('copy', 'public/sites/all/modules/custom/ombudashboard/ombudashboard.admin_blocks.inc.example', 'public/sites/default/ombudashboard.admin_blocks.inc');
  }

  /**
   * Setup git.
   */
  protected function setupGit() {
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

    // Setup each git repo in make file as a submodule.
    foreach ($this->settings->makeFiles as $file) {
      $make_file = $this->settings->manifestPath . $file . '.make';
      $info = make_parse_info_file($make_file);

      foreach ($info['projects'] as $name => $project) {
        if (isset($project['download']['type']) && $project['download']['type'] == 'git') {

          $path = sprintf('public/sites/all/%s/%s%s',
            $project['type'] . 's',
            isset($project['subdir']) ? $project['subdir'] . '/' : '',
            $name
          );

          drush_shell_exec('git submodule add %s %s',
            $project['download']['url'],
            $path
          );
        }
      }
    }

    drush_shell_exec('git add public fabfile.py README.md .gitignore');
    drush_shell_exec('git add -f public/sites/default/settings.*');
    drush_shell_exec('git commit -m "Initial install of OMBU Core"');
  }
}
