// $Id: ib_wysiwyg.js,v 1.1.2.1 2009/10/24 17:04:41 starnox Exp $

/**
 * @file ib_wysiwyg.js
 *
 * Javascript plugin for Image Browser.
 *
 * Variables:
 * - data.fid             [File ID]
 * - data.preset          [ImageCache preset name]
 * - data.link_preset     [ImageCache preset name]
 * - data.link_target     [Link target]
 * - data.styles          [CSS classes separated by spaces]
 * - data.filepath        [Filepath to image relative to drupal]
 * - data.image_url       [Full URL to image]
 * - data.link_url        [Full URL to link]
 */

function ib_process_insert(data) {
  // Get WYSIWYG editor instance.
  var instanceId = parent.instanceId;

  // Insert tag.
  parent.Drupal.wysiwyg.instances[instanceId].insert('<img src="'+data.image_url+'" alt="IB Image" title="'+data.fid+'=='+data.preset+'=='+data.link_preset+'=='+data.link_target+'=='+data.styles+'" class="ibimage '+data.styles+'" />');

  // Close jQuery UI dialog.
  parent.ib_wysiwyg_dialog_close();
}