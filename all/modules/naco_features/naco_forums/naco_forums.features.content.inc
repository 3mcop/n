<?php

/**
 * Implementation of hook_content_default_fields().
 */
function naco_forums_content_default_fields() {
  $fields = array();

  // Exported field: field_advert_image
  $fields['forum_advert-field_advert_image'] = array(
    'field_name' => 'field_advert_image',
    'type_name' => 'forum_advert',
    'display_settings' => array(
      'weight' => '-4',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'ib_thumbnail_imagelink',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'ib_thumbnail_imagelink',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'filefield',
    'required' => '1',
    'multiple' => '0',
    'module' => 'filefield',
    'active' => '1',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '0',
    'widget' => array(
      'file_extensions' => 'jpg jpeg gif png',
      'file_path' => 'forum-adverts',
      'progress_indicator' => 'bar',
      'max_filesize_per_file' => '1M',
      'max_filesize_per_node' => '1M',
      'label' => 'Advert image',
      'weight' => '-4',
      'description' => 'choose an image file to upload from your computer.',
      'type' => 'filefield_widget',
      'module' => 'filefield',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Advert image');

  return $fields;
}
