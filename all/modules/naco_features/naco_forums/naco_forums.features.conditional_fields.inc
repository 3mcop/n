<?php

/**
 * Implementation of hook_conditional_fields_default_fields().
 */
function naco_forums_conditional_fields_default_fields() {
$items = array();
$items[] = array (
  'control_field_name' => 'field_advert_all_forums',
  'field_name' => 'field_advert_forums',
  'type' => 'forum_advert',
  'trigger_values' => 
  array (
    0 => '0',
  ),
);
return $items;
}
