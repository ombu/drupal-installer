; Core make file for OMBU projects in Drupal 7.x

core = 7.x

api = 2
projects[] = drupal

; Profiles
projects[ombuprofile][download][type]   = git
projects[ombuprofile][download][url]    = git@github.com:ombu/drupal-ombuprofile.git
projects[ombuprofile][download][branch] = d7
projects[ombuprofile][type]             = profile

; Contrib Modules
projects[views_bulk_operations][subdir] = contrib
projects[entity][subdir]                = contrib
projects[ctools][subdir]                = contrib
projects[context][subdir]               = contrib
projects[devel][subdir]                 = contrib
projects[diff][subdir]                  = contrib
projects[features][subdir]              = contrib
projects[pathauto][subdir]              = contrib
projects[token][subdir]                 = contrib
projects[views][subdir]                 = contrib
projects[wysiwyg][subdir]               = contrib
projects[media][subdir]                 = contrib
projects[xmlsitemap][subdir]            = contrib

; Libraries
libraries[tinymce][download][type] = get
libraries[tinymce][download][url]  = https://github.com/downloads/tinymce/tinymce/tinymce_3.3.9.4.zip
