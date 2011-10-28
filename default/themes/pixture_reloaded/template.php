<?php // $Id: template.php,v 1.8 2009/05/04 21:34:31 jmburnz Exp $

/**
 * @file
 *  template.php
 */

/**
 * Initialize theme settings for page width.
 */
$pixture_width = theme_get_setting('pixture_width');
pixture_validate_page_width($pixture_width);

/**
 * Check the page width theme settings and reset to default
 * if the value is null, or invalid value is specified
 */
function pixture_validate_page_width($width) {
  global $theme_key;

  /**
   * The default values for the theme variables. Make sure $defaults exactly
   * matches the $defaults in the theme-settings.php file.
   */
  $defaults = array(             // <-- change this array
    'pixture_width' => '85%',
  );

  // check if it is liquid (%) or fixed width (px)
  if (preg_match("/(\d+)\s*%/", $width, $match)) {
    $liquid = 1;
    $num = intval($match[0]);
    if (50 <= $num && $num <= 100) {
      return $num ."%";  // OK!
    }
  }
  else if (preg_match("/(\d+)\s*px/", $width, $match)) {
    $fixed = 1;
    $num = intval($match[0]);
    if (800 <= $num && $num < 1600) {
      return $num ."px"; // OK
    }
  }

  // reset to default value
  variable_set(
    str_replace('/', '_', 'theme_'. $theme_key .'_settings'),
    array_merge($defaults, theme_get_settings($theme_key))
  );
  // Force refresh of Drupal internals
  theme_get_setting('', TRUE);
  return $defaults['pixture_width'];
}

/**
 * Initialize theme settings for superfish.
 */
if (is_null(theme_get_setting('pixture_superfish'))) {  // <-- change this line
  global $theme_key;

  /**
   * The default values for the theme variables. Make sure $defaults exactly
   * matches the $defaults in the theme-settings.php file.
   */
  $defaults = array(             // <-- change this array
    'pixture_superfish' => 0,
  );

  // Get default theme settings.
  $settings = theme_get_settings($theme_key);

  // Save default theme settings.
  variable_set(
    str_replace('/', '_', 'theme_'. $theme_key .'_settings'),
    array_merge($defaults, $settings)
  );
  // Force refresh of Drupal internals.
  theme_get_setting('', TRUE);
  return $defaults['pixture_superfish'];
}
// Conditionally load the Superfish JS
if (theme_get_setting('pixture_superfish')) {
  drupal_add_css(drupal_get_path('theme', 'pixture_reloaded') .'/sf/css/superfish.css', 'theme', 'all', FALSE);
  drupal_add_js(drupal_get_path('theme', 'pixture_reloaded') .'/sf/js/superfish.js', 'theme');
}

/**
 * Override or insert PHPTemplate variables into the page templates.
 *
 * @param $vars
 *   A sequential array of variables to pass to the theme template.
 * @param $hook
 *   The name of the theme function being called ("page" in this case.)
 */
function pixture_reloaded_preprocess_page(&$vars, $hook) {
  global $theme;

  // Hook into color.module
  if (module_exists('color')) {
    _color_page_alter($vars);
  }

  // Don't display empty help from node_help().
  if ($vars['help'] == "<div class=\"help\"><p></p>\n</div>") {
    $vars['help'] = '';
  }
  
  // Set variables for the logo and site_name.
  if (!empty($vars['logo'])) {
    $vars['site_logo'] = '<a href="'. $vars['front_page'] .'" title="'. t('Home page') .'" rel="home"><img src="'. $vars['logo'] .'" alt="'. $vars['site_name'] .' '. t('logo') .'" /></a>';
  }
  if (!empty($vars['site_name'])) {
    $vars['site_name'] = '<a href="'. $vars['front_page'] .'" title="'. t('Home page') .'" rel="home">'. $vars['site_name'] .'</a>';
  }
  
  // Set variables for the primary and secondary links.
  if (!empty($vars['primary_links'])) {
    $vars['primary_menu'] = theme('links', $vars['primary_links'], array('class' => 'links primary-links'));
  }

  // Classes for body element. Allows advanced theming based on context
  // (home page, node of certain type, etc.)
  $body_classes = array($vars['body_classes']);
    if (!$vars['is_front']) {
      // Add unique classes for each page and website section
      $path = drupal_get_path_alias($_GET['q']);
      list($section, ) = explode('/', $path, 2);
      $body_classes[] = pixture_reloaded_id_safe('page-'. $path);
      $body_classes[] = pixture_reloaded_id_safe('section-'. $section);
      if (arg(0) == 'node') {
        if (arg(1) == 'add') {
          if ($section == 'node') {
            array_pop($body_classes); // Remove 'section-node'
          }
          $body_classes[] = 'section-node-add'; // Add 'section-node-add'
        }
        elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
          if ($section == 'node') {
            array_pop($body_classes); // Remove 'section-node'
          }
          $body_classes[] = 'section-node-'. arg(2); // Add 'section-node-edit' or 'section-node-delete'
        }
      }
    // Add a unique class when viewing a node
    if (arg(0) == 'node' && is_numeric(arg(1))) {
      $body_classes[] = 'node-full-view'; // Add 'node-full-view'
    }
  }
  $vars['body_classes'] = implode(' ', $body_classes); // Concatenate with spaces
}

/**
 * Override or insert PHPTemplate variables into the node templates.
 *
 * @param $vars
 *   A sequential array of variables to pass to the theme template.
 * @param $hook
 *   The name of the theme function being called ("node" in this case.)
 */
function pixture_reloaded_preprocess_node(&$vars, $hook) {
  global $user;

  // Special classes for nodes
  $node_classes = array();
    if ($vars['sticky']) {
      $node_classes[] = 'sticky';
    }
    if (!$vars['node']->status) {
      $node_classes[] = 'node-unpublished';
      $vars['unpublished'] = TRUE;
    }
    else {
      $vars['unpublished'] = FALSE;
    }
    if ($vars['node']->uid && $vars['node']->uid == $user->uid) {
      // Node is authored by current user
      $node_classes[] = 'node-mine';
    }
    if ($vars['teaser']) {
      // Node is displayed as teaser
      $node_classes[] = 'node-teaser';
    }
    if ($vars['$is_front']) {
      // Node is displayed on the front page
      $node_classes[] = 'front-node';
    }
  // Class for node type: "node-type-page", "node-type-story", "node-type-my-custom-type", etc.
  $node_classes[] = 'node-type-'. $vars['node']->type;
  $vars['node_classes'] = implode(' ', $node_classes); // Concatenate with spaces
}

/**
 * Override or insert PHPTemplate variables into the comment templates.
 *
 * @param $vars
 *   A sequential array of variables to pass to the theme template.
 * @param $hook
 *   The name of the theme function being called ("comment" in this case.)
 */
function pixture_reloaded_preprocess_comment(&$vars, $hook) {
  global $user;

  // We load the node object that the current comment is attached to
  $node = node_load($vars['comment']->nid);
  // If the author of this comment is equal to the author of the node, we
  // set a variable so we can theme this comment uniquely.
  $vars['author_comment'] = $vars['comment']->uid == $node->uid ? TRUE : FALSE;

  $comment_classes = array();

  // Odd/even handling
  static $comment_odd = TRUE;
  $comment_classes[] = $comment_odd ? 'odd' : 'even';
  $comment_odd = !$comment_odd;

  if ($vars['comment']->status == COMMENT_NOT_PUBLISHED) {
    $comment_classes[] = 'comment-unpublished';
    $vars['unpublished'] = TRUE;
  }
  else {
    $vars['unpublished'] = FALSE;
  }
  if ($vars['author_comment']) {
    // Comment is by the node author
    $comment_classes[] = 'comment-by-author';
  }
  if ($vars['comment']->uid == 0) {
    // Comment is by an anonymous user
    $comment_classes[] = 'comment-by-anon';
  }
  if ($user->uid && $vars['comment']->uid == $user->uid) {
    // Comment was posted by current user
    $comment_classes[] = 'comment-mine';
  }
  $vars['comment_classes'] = implode(' ', $comment_classes);

  // If comment subjects are disabled, don't display 'em
  if (variable_get('comment_subject_field', 1) == 0) {
    $vars['title'] = '';
  }
}

/**
 * Override or insert PHPTemplate variables into the block templates.
 *
 * @param $vars
 *   A sequential array of variables to pass to the theme template.
 * @param $hook
 *   The name of the theme function being called ("block" in this case.)
 */
function pixture_reloaded_preprocess_block(&$vars, $hook) {
  $block = $vars['block'];

  // Special classes for blocks
  $block_classes = array();
  $block_classes[] = 'block-'. $block->module;
  $block_classes[] = 'region-'. $vars['block_zebra'];
  $block_classes[] = $vars['zebra'];
  $block_classes[] = 'region-count-'. $vars['block_id'];
  $block_classes[] = 'count-'. $vars['id'];
  $vars['block_classes'] = implode(' ', $block_classes);

}

/**
 * Converts a string to a suitable html ID attribute.
 *
 * - Preceeds initial numeric with 'n' character.
 * - Replaces space and underscore with dash.
 * - Converts entire string to lowercase.
 * - Works for classes too!
 *
 * @param string $string
 *   The string
 * @return
 *   The converted string
 */
function pixture_reloaded_id_safe($string) {
  if (is_numeric($string{0})) {
    // If the first character is numeric, add 'n' in front
    $string = 'n'. $string;
  }
  return strtolower(preg_replace('/[^a-zA-Z0-9-]+/', '-', $string));
}