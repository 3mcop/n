// $Id: ib_fckeditor.js,v 1.1.2.2 2009/10/24 17:04:41 starnox Exp $

/**
 * @file ib_fckeditor.js
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
  var FCK = parent.FCK_instance;
  
  var oImage = FCK.Selection.GetSelectedElement() ;

  if ( oImage && oImage.tagName.toLowerCase() != 'img' )
  oImage = null ;
  
  var bHasImage = ( oImage != null ) ;
  
  if ( !bHasImage )
  {
    oImage = FCK.InsertElement( 'img' ) ;
  }
  
  $(oImage).attr("src", data.image_url);
  $(oImage).attr("alt", 'IB Image');
  $(oImage).attr("class", data.styles);
  $(oImage).attr("_fck_ib", data.fid + '==' + data.preset + '==' + data.link_preset + '==' + data.link_target + '==' + data.styles);
  
  parent.ib_fckeditor_dialog_close();
}