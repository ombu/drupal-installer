#! /bin/sh

################################################################################
# Command line args
################################################################################
# set flag vars to empty
noinstall=
dev=

while getopts :nd opt
do
    case $opt in
    d)     dev=true
           ;;
    n)     noinstall=true
           ;;
    esac
done

################################################################################
# Variables needed to launch a Drupal site
################################################################################
PROFILE="ombuprofile"
DBNAME="__DBNAME__"
DBUSER="__DBUSER__"
DBPW="__DBPASS__"
SITENAME="__SITENAME__"
SITEMAIL="__SITEMAIL__"
ACCTNAME="system"
ACCTPW="pass"
ACCTMAIL="__SITEMAIL__"

################################################################################
# Reset DB
################################################################################
printf "Enter mysql root password: "
stty -echo; read -r MYSQLROOT; stty echo  # won't display what user types
echo ""

SQL="
DROP DATABASE IF EXISTS $DBNAME;
CREATE DATABASE $DBNAME;
GRANT SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, INDEX, ALTER, CREATE
TEMPORARY TABLES, LOCK TABLES ON $DBNAME.* TO '$DBUSER'@'$DBHOST' IDENTIFIED BY '$DBPW';
"

echo "+ Reseting databse..."
echo $SQL | mysql -u root -p$MYSQLROOT

################################################################################
# Should not need to change code below
################################################################################

if [ $noinstall ]; then
  echo "Called with noinstall option. Exiting before running installer."
  exit 0
fi

echo "+ Running Drupal installer..."
drush si --yes $PROFILE             \
  --db-url="mysqli://$DBUSER:$DBPW@localhost/$DBNAME" \
  --site-name="$SITENAME"         \
  --site-mail=$SITEMAIL           \
  --account-name=$ACCTNAME        \
  --account-pass=$ACCTPW          \
  --account-mail=$ACCTMAIL        \
  --dbuser=$DBUSER

if [ $dev ]; then
  echo "+ Installing dev modules";

  drush en -y devel devel_generate context_ui views_ui diff node_export node_export_file

  # Generate test content
  # Usage: drush genc --types=node_type number_of_nodes number_of_comments
  # drush genc --types=page 20

  # Import exported content (with file assets).  Usage:
  #   drush vset --yes node_export_file_assets_path path_to_assets_directory
  #   drush node-export-import --file=path_to_export_file
  # drush vset --yes node_export_file_assets_path profiles/ombubase/exports/assets
  # drush node-export-import --file=profiles/ombubase/exports/ombubase-node_export.inc
fi

drush cc all
chmod 0755 sites/default
chmod 0644 sites/default/settings.php

if [ -x growlnotify ]; then
  growlnotify -a Terminal -m "$SITENAME Drupal build complete"
fi
