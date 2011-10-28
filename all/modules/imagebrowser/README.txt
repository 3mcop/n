# $Id: README.txt,v 1.3.2.3 2009/10/24 17:04:41 starnox Exp $

==============================
Required Modules
==============================

 - Image API (http://drupal.org/project/imageapi)
 - ImageCache (http://drupal.org/project/imagecache)
 - Views (http://drupal.org/project/views)

Optional Modules (for Image Browser v1 backwards compatibility only):
------------------------------

 - Image (http://drupal.org/project/image)

==============================
Installation
==============================

 1. Drop the "imagebrowser" folder into the modules directory (/sites/all/modules/)
 2. Enable the "Image Browser" module (?q=/admin/build/modules)
 3. Enable at least one Image Browser Plugin (?q=/admin/build/modules)
 4. Add the "Image Browser Images" filter to one of your Input Formats (?q=admin/settings/filters/list)
 5. Setup your permissions (?q=/admin/user/permissions)

==============================
Upgrading
==============================

There is no upgrade path from Image Browser v1 yet. As long as you leave the Image module installed however then all previously inserted images should continue to work. You MUST completely un-install and re-install the "Image Browser" module if moving from v1 to v2 (un-install via the modules page (?q=admin/build/modules/uninstall).

==============================
To Do
==============================

 - Write upgrade documentation for converting images previously inserted by Image Browser v1 to the all new Image Browser filter tags in v2.

==============================
Support
==============================

If you have any questions, issues, or feature suggestions then please do leave feedback on the project page (http://drupal.org/project/imagebrowser)