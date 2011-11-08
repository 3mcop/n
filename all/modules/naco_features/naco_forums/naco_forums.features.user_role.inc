<?php

/**
 * Implementation of hook_user_default_roles().
 */
function naco_forums_user_default_roles() {
  $roles = array();

  // Exported role: forum sponsor
  $roles['forum sponsor'] = array(
    'name' => 'forum sponsor',
  );

  // Exported role: forum user
  $roles['forum user'] = array(
    'name' => 'forum user',
  );

  return $roles;
}
