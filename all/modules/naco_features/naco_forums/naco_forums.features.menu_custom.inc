<?php

/**
 * Implementation of hook_menu_default_menu_custom().
 */
function naco_forums_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-forum
  $menus['menu-forum'] = array(
    'menu_name' => 'menu-forum',
    'title' => 'Forum',
    'description' => 'links to appear on furum pages.',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Forum');
  t('links to appear on furum pages.');


  return $menus;
}
