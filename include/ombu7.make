; Core make file for OMBU projects in Drupal 7.x

core = 7.x

api = 2
projects[] = drupal

; Profiles
projects[ombuprofile][download][type]   = git
projects[ombuprofile][download][url]    = git@github.com:ombu/drupal-ombuprofile.git
projects[ombuprofile][download][branch] = feature-inherited
projects[ombuprofile][type]             = profile

; Contrib Modules
projects[views][subdir]                 = contrib
projects[views_bulk_operations][subdir] = contrib
projects[wysiwyg][subdir]               = contrib
projects[pathauto][subdir]              = contrib
projects[redirect][subdir]              = contrib
projects[context][subdir]               = contrib
projects[media][subdir]                 = contrib
projects[oembed][subdir]                = contrib
projects[entity][subdir]                = contrib
projects[ctools][subdir]                = contrib
projects[features][subdir]              = contrib
projects[token][subdir]                 = contrib
projects[xmlsitemap][subdir]            = contrib


; OMBU Modules
projects[ombucore][subdir]                = custom
projects[ombucore][download][type]        = git
projects[ombucore][download][url]         = git@github.com:ombu/drupal-ombucore.git
projects[ombucore][download][branch]      = master
projects[ombucore][type]                  = module

projects[ombucleanup][subdir]             = custom
projects[ombucleanup][download][type]     = git
projects[ombucleanup][download][url]      = git@github.com:ombu/drupal-ombucleanup.git
projects[ombucleanup][download][branch]   = drupal7
projects[ombucleanup][type]               = module

projects[ombudashboard][subdir]           = custom
projects[ombudashboard][download][type]   = git
projects[ombudashboard][download][url]    = git@github.com:ombu/drupal-ombudashboard.git
projects[ombudashboard][download][branch] = drupal7
projects[ombudashboard][type]             = module

projects[ombuseo][subdir]                 = custom
projects[ombuseo][download][type]         = git
projects[ombuseo][download][url]          = git@github.com:ombu/drupal-ombuseo.git
projects[ombuseo][download][branch]       = drupal7
projects[ombuseo][type]                   = module

; Libraries
libraries[tinymce][download][type] = get
libraries[tinymce][download][url]  = https://github.com/downloads/tinymce/tinymce/tinymce_3.3.9.4.zip
