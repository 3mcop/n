<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function naco_forums_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'forums_except_top_level';
  $context->description = 'Every forum (and "container"). This does not include the top level forum page (which is not in a forum).';
  $context->tag = 'Forums';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'forum/*' => 'forum/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-forum_adverts_admin-block_3' => array(
          'module' => 'views',
          'delta' => 'forum_adverts_admin-block_3',
          'region' => 'content_top',
          'weight' => 0,
        ),
        'views-forum_adverts_admin-block_1' => array(
          'module' => 'views',
          'delta' => 'forum_adverts_admin-block_1',
          'region' => 'content_top',
          'weight' => 1,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Every forum (and "container"). This does not include the top level forum page (which is not in a forum).');
  t('Forums');
  $export['forums_except_top_level'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'forums_top_level_for_all';
  $context->description = 'Only the top level page of the forums, for everyone.';
  $context->tag = 'Forums';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'forum' => 'forum',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-forum_adverts_admin-block_2' => array(
          'module' => 'views',
          'delta' => 'forum_adverts_admin-block_2',
          'region' => 'content_top',
          'weight' => -10,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Forums');
  t('Only the top level page of the forums, for everyone.');
  $export['forums_top_level_for_all'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'forums_top_level_for_anon';
  $context->description = 'Only the top level page of the forums, only for anonymous users (+admins so they get edit/configure links.)';
  $context->tag = 'Forums';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'forum' => 'forum',
      ),
    ),
    'user' => array(
      'values' => array(
        'NACO Admin' => 'NACO Admin',
        'anonymous user' => 'anonymous user',
        'site administrator' => 'site administrator',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'nodeblock-105' => array(
          'module' => 'nodeblock',
          'delta' => 105,
          'region' => 'content_top',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Forums');
  t('Only the top level page of the forums, only for anonymous users (+admins so they get edit/configure links.)');
  $export['forums_top_level_for_anon'] = $context;

  return $export;
}
