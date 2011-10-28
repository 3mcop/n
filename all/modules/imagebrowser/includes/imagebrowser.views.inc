<?php
// $Id: imagebrowser.views.inc,v 1.1.4.3 2009/10/24 17:04:41 starnox Exp $

/**
 * @file
 * Image Browser's views plugins
 */


/**
 * Implementation of hook_views_plugins().
 *
 * Define a row style plugin to display a thumbnail in the image browser.
 */
function imagebrowser_views_plugins() {
  return array(
    'row' => array(
      'imagebrowser_thumbnail' => array(
        'title' => t('Image Browser thumbnail'),
        'handler' => 'views_plugin_row',
        'theme' => 'imagebrowser_thumbnail',
        'theme path' => drupal_get_path('module', 'imagebrowser') .'/theme',
        'theme file' => 'theme.inc',
        'uses options' => FALSE,
        'type' => 'normal',
        'uses fields' => TRUE,
      ),
    ),
  );
}