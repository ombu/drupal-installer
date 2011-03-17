; Core make file for OMBU projects in Drupal 7.x

core = 7.x

api = 2
projects[] = drupal

; Profiles
projects[ombubase][download][type]   = git
projects[ombubase][download][url]    = git@github.com:ombu/drupal-ombubase.git
projects[ombubase][download][branch] = d7
projects[ombubase][type]             = profile

; Contrib Modules
projects[views_bulk_operations][subdir] = contrib
projects[ctools][subdir]                = contrib
projects[context][subdir]               = contrib
projects[devel][subdir]                 = contrib
projects[diff][subdir]                  = contrib
projects[features][subdir]              = contrib
projects[pathauto][subdir]              = contrib
projects[token][subdir]                 = contrib
projects[views][subdir]                 = contrib
projects[views_ui_basic][subdir]        = contrib
projects[wysiwyg][subdir]               = contrib

; Custom Modules
projects[ombucleanup][download][type]   = git
projects[ombucleanup][download][url]    = git@github.com:ombu/drupal-ombucleanup.git
projects[ombucleanup][download][branch] = drupal7
projects[ombucleanup][type]             = module
projects[ombucleanup][subdir]           = custom

projects[ombudashboard][download][type]   = git
projects[ombudashboard][download][url]    = git@github.com:ombu/drupal-ombudashboard.git
projects[ombudashboard][download][branch] = drupal7
projects[ombudashboard][type]             = module
projects[ombudashboard][subdir]           = custom

; Themes
projects[ombuadmin][download][type]   = git
projects[ombuadmin][download][url]    = git@github.com:ombu/drupal-ombuadmin.git
projects[ombuadmin][download][branch] = d7
projects[ombuadmin][type]             = theme

; Libraries
libraries[tinymce][download][type] = git
libraries[tinymce][download][url]  = https://github.com/tinymce/tinymce.git
libraries[tinymce][download][tag]  = 3.4
