; Core make file for OMBU projects in Drupal 7.x

core = 7.x

api = 2
projects[] = drupal

; Contrib Modules
projects[bean][subdir]                    = contrib
projects[context][subdir]                 = contrib
projects[ctools][subdir]                  = contrib
projects[devel][subdir]                   = contrib
projects[diff][subdir]                    = contrib
projects[entity][subdir]                  = contrib
projects[features][subdir]                = contrib
projects[field_collection][subdir]        = contrib
projects[htmlpurifier][subdir]            = contrib
projects[jquery_update][subdir]           = contrib
projects[libraries][subdir]               = contrib
projects[link][subdir]                    = contrib
projects[oembed][subdir]                  = contrib
projects[pathauto][subdir]                = contrib
projects[redirect][subdir]                = contrib
projects[token][subdir]                   = contrib
projects[views][subdir]                   = contrib
projects[views_bulk_operations][subdir]   = contrib
projects[xmlsitemap][subdir]              = contrib

projects[media][subdir]                   = contrib
projects[media][version]                  = 2.0-alpha1
projects[file_entity][subdir]             = contrib

projects[wysiwyg][subdir]                 = contrib
projects[wysiwyg][patches][]              = https://drupal.org/files/wysiwyg-ckeditor-4.1853550.136.patch

; OMBU Modules
projects[ombucore][subdir]                = custom
projects[ombucore][download][type]        = git
projects[ombucore][download][url]         = git@github.com:ombu/drupal-ombucore.git
projects[ombucore][download][branch]      = develop
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

projects[ombubeans][subdir]               = custom
projects[ombubeans][download][type]       = git
projects[ombubeans][download][url]        = git@github.com:ombu/drupal-ombubeans.git
projects[ombubeans][download][branch]     = develop
projects[ombubeans][type]                 = module

projects[beancontainer][subdir]           = custom
projects[beancontainer][download][type]   = git
projects[beancontainer][download][url]    = git@github.com:ombu/drupal-beancontainer.git
projects[beancontainer][download][branch] = master
projects[beancontainer][type]             = module

projects[tiles][subdir]                   = custom
projects[tiles][download][type]           = git
projects[tiles][download][url]            = git@github.com:ombu/drupal-tiles.git
projects[tiles][download][branch]         = develop
projects[tiles][type]                     = module

projects[ombuslide][subdir]               = custom
projects[ombuslide][download][type]       = git
projects[ombuslide][download][url]        = git@github.com:ombu/drupal-ombuslide.git
projects[ombuslide][download][branch]     = master
projects[ombuslide][type]                 = module

; OMBU Themes
projects[boots][download][type]           = git
projects[boots][download][url]            = git@github.com:ombu/drupal-boots.git
projects[boots][download][branch]         = master
projects[boots][type]                     = theme

; Libraries
libraries[ckeditor][download][type]       = get
libraries[ckeditor][download][url]        = http://download.cksource.com/CKEditor/CKEditor/CKEditor%204.2.1/ckeditor_4.2.1_full.zip
libraries[htmlpurifier][download][type]   = get
libraries[htmlpurifier][download][url]    = http://htmlpurifier.org/releases/htmlpurifier-4.5.0.zip
