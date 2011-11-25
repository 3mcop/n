<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function naco_forums_user_default_permissions() {
  $permissions = array();

  // Exported permission: administer advanced forum
  $permissions['administer advanced forum'] = array(
    'name' => 'administer advanced forum',
    'roles' => array(
      '0' => 'NACO Admin',
    ),
  );

  // Exported permission: administer forums
  $permissions['administer forums'] = array(
    'name' => 'administer forums',
    'roles' => array(
      '0' => 'NACO Admin',
    ),
  );

  // Exported permission: create forum topics
  $permissions['create forum topics'] = array(
    'name' => 'create forum topics',
    'roles' => array(
      '0' => 'NACO Admin',
      '1' => 'forum sponsor',
    ),
  );

  // Exported permission: delete any forum topic
  $permissions['delete any forum topic'] = array(
    'name' => 'delete any forum topic',
    'roles' => array(
      '0' => 'NACO Admin',
    ),
  );

  // Exported permission: delete own forum topics
  $permissions['delete own forum topics'] = array(
    'name' => 'delete own forum topics',
    'roles' => array(
      '0' => 'NACO Admin',
    ),
  );

  // Exported permission: edit any forum topic
  $permissions['edit any forum topic'] = array(
    'name' => 'edit any forum topic',
    'roles' => array(
      '0' => 'NACO Admin',
    ),
  );

  // Exported permission: edit own forum topics
  $permissions['edit own forum topics'] = array(
    'name' => 'edit own forum topics',
    'roles' => array(
      '0' => 'NACO Admin',
      '1' => 'forum sponsor',
    ),
  );

  // Exported permission: view forum statistics
  $permissions['view forum statistics'] = array(
    'name' => 'view forum statistics',
    'roles' => array(
      '0' => 'NACO Admin',
    ),
  );

  return $permissions;
}
