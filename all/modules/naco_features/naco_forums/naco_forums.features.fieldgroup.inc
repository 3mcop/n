<?php

/**
 * Implementation of hook_fieldgroup_default_groups().
 */
function naco_forums_fieldgroup_default_groups() {
  $groups = array();

  // Exported group: group_advert_where
  $groups['forum_advert-group_advert_where'] = array(
    'group_type' => 'standard',
    'type_name' => 'forum_advert',
    'group_name' => 'group_advert_where',
    'label' => 'Where to display advert',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-2',
    'fields' => array(
      '0' => 'field_advert_top_level',
      '1' => 'field_advert_all_forums',
      '2' => 'field_advert_forums',
    ),
  );

  // Exported group: group_profile
  $groups['forum_user_profile-group_profile'] = array(
    'group_type' => 'standard',
    'type_name' => 'forum_user_profile',
    'group_name' => 'group_profile',
    'label' => 'Forum user profile',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '-4',
    'fields' => array(
      '0' => 'field_fup_name',
      '1' => 'field_fup_organisation',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Forum user profile');
  t('Where to display advert');

  return $groups;
}
