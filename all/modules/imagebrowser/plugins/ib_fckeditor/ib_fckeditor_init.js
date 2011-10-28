// $Id: ib_fckeditor_init.js,v 1.1.2.1 2009/12/05 10:09:14 starnox Exp $

/**
 * @file ib_fckeditor_init.js
 *
 * All the javascript for the FCKeditor plugin for Image Browser.
 */

var FCK_instance = null;

/**
 * Open jQuery UI dialog box.
 */
function ib_fckeditor_dialog_open(instance) {
  FCK_instance = instance;
  $("body").append("<div id='ib_dialog'><iframe id='ib_dialog_contents' src='"+Drupal.settings.imagebrowser.basepath + "?q=imagebrowser/ib_fckeditor' width='602' height='446' frameborder='0' /></div>");
  $("#ib_dialog").dialog({ modal: true, height: 476, width: 602, title: 'Image Browser', beforeclose: function(event, ui) { ib_fckeditor_dialog_close(); } }).show();
}

/**
 * Close jQuery UI dialog box.
 */
function ib_fckeditor_dialog_close() {
  $("#ib_dialog").remove();
}