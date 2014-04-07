from fabric.api import task, env, cd
from butter import drush, deploy, drupal
from fabric.contrib import files
from fabric.operations import run

env.repo_type = 'git'
env.repo_url = 'git@bitbucket.org:ombu/CHANGEME.git'
env.use_ssh_config = 'true'

# Global environment settings
env.site_name = '__SITENAME__'
env.db_db = '__DBNAME__'
env.public_path = 'public'
env.site_profile = '__PROFILENAME__'

# Dev modules to install during site build
env.dev_modules = 'devel devel_generate context_ui views_ui diff bean_admin_ui'

# Host settings
@task
def local():
    """
    The local host definition
    """
    env.db_user = 'phpuser'
    env.db_pw = 'meh'
    env.db_host = 'localhost'

def qa():
    """
    The qa server definition
    """
    # TODO: fill out this info with the correct QA specs
    # env.hosts = ['pepe.ombuweb.com']
    # env.host_type = 'qa'
    # env.host_webserver_user = 'www-data'
    # env.host_site_path = '/var/www/qaX'
    # env.public_path = 'current'
    # env.db_db = 'qaX'
    # env.db_user = 'phpuser'
    # env.db_pw = 'meh'
    # env.db_host = 'qadb.cpuj5trym3at.us-west-2.rds.amazonaws.com'

@task
def staging():
    """
    The staging server definition
    """
    # TODO: fill out this info with the correct Staging specs
    # env.hosts = ['pepe.ombuweb.com']
    # env.host_type = 'staging'
    # env.host_webserver_user = 'www-data'
    # env.host_site_path = '/var/www/stageX'
    # env.public_path = 'current'
    # env.db_db = 'stageX'
    # env.db_user = 'phpuser'
    # env.db_pw = 'meh'
    # env.db_host = 'qadb.cpuj5trym3at.us-west-2.rds.amazonaws.com'
