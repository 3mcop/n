<?php

/**
 * Implementation of hook_imagecache_default_presets().
 */
function naco_forums_imagecache_default_presets() {
  $items = array(
    'full-size' => array(
      'presetname' => 'full-size',
      'actions' => array(),
    ),
    'ib_thumbnail' => array(
      'presetname' => 'ib_thumbnail',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => 100,
            'height' => 100,
            'upscale' => 1,
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_node_info().
 */
function naco_forums_node_info() {
  $items = array(
    'forum_advert' => array(
      'name' => t('Forum advert'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'forum_user_profile' => array(
      'name' => t('Forum user profile'),
      'module' => 'features',
      'description' => t('Public information about a forum user.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function naco_forums_views_api() {
  return array(
    'api' => '2',
  );
}
