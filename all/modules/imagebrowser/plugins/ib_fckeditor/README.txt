# $Id: README.txt,v 1.1.2.1 2009/12/05 10:09:14 starnox Exp $

==============================
Required Modules
==============================

 - Image Browser (http://drupal.org/project/imagebrowser)
 - jQuery UI (http://drupal.org/project/jquery_ui)
 - FCKeditor (http://drupal.org/project/fckeditor)

==============================
Installation
==============================

 1. Enable the "Image Browser Plugin - FCKeditor" module (?q=/admin/build/modules)
 2. Enable FCKeditor (IB plugin) module for use with FCKeditor (?q=/admin/build/modules)
 3. Move the FCKeditor Image Browser plugin (/sites/all/modules/imagebrowser/plugins/ib_fckeditor/imagebrowser) into the FCKeditor plugins directory (/sites/all/modules/fckeditor/plugins/) so you should have (/sites/all/modules/fckeditor/plugins/imagebrowser/fckplugin.js)
 4. Edit the fckeditor.config.js file and add this line (/sites/all/modules/fckeditor/fckeditor.config.js)
    | FCKConfig.Plugins.Add('imagebrowser');

 5. Edit the FCKeditor global profile and add "admin/settings/imagebrowser/add-new-style" to 'paths to include/exclude' under 'Visibility settings' (?q=/admin/settings/fckeditor/)
 6. Edit a FCKeditor profile and set 'File browser type' to "Image Browser" under 'File Browser settings' (?q=admin/settings/fckeditor)
 7. Setup your permissions (?q=/admin/user/permissions)

==============================
Upgrading
==============================

When upgrading please repeat step 3 above.

==============================
To Do
==============================

 - Check compatibility with other browsers (only tested in Safari 4).

==============================
Support
==============================

If you have any questions, issues, or feature suggestions then please do leave feedback on the project page (http://drupal.org/project/imagebrowser)