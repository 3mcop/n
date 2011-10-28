<?php
// $Id: imagebrowser.views_default.inc,v 1.1.4.6 2010/02/10 14:29:13 blixxxa Exp $

/**
 * @file
 * Image Browser's default views
 */


/**
 * Implementation of hook_views_default_views().
 */
function imagebrowser_views_default_views() {
  $views = array();
  
  $view = new view;
  $view->name = 'ib_browser';
  $view->description = 'File browser for Image Browser';
  $view->tag = 'Image Browser';
  $view->view_php = '';
  $view->base_table = 'users';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'filepath' => array(
      'label' => '',
      'link_to_file' => 0,
      'exclude' => 0,
      'id' => 'filepath',
      'table' => 'files',
      'field' => 'filepath',
      'relationship' => 'none',
    ),
    'fid' => array(
      'label' => 'File ID',
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_file' => 0,
      'exclude' => 1,
      'id' => 'fid',
      'table' => 'files',
      'field' => 'fid',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'timestamp' => array(
      'order' => 'DESC',
      'granularity' => 'second',
      'id' => 'timestamp',
      'table' => 'files',
      'field' => 'timestamp',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'uid' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'php',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'uid',
      'table' => 'users',
      'field' => 'uid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        '2' => 0,
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => 'global $user;
  if($user->uid == 1 || user_access(\'browse all images\')):
  return \'all\';
  elseif(user_access(\'browse own images\')):
  return $user->uid;
  else:
  return 0;
  endif;',
      'validate_argument_node_type' => array(
        'page' => 0,
        'story' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('filters', array(
    'filemime' => array(
      'operator' => 'word',
      'value' => 'jpg jpeg png gif',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'case' => 1,
      'id' => 'filemime',
      'table' => 'files',
      'field' => 'filemime',
      'relationship' => 'none',
    ),
    'status' => array(
      'operator' => 'in',
      'value' => array(
        '1' => '1',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'files',
      'field' => 'status',
      'relationship' => 'none',
    ),
    'filepath' => array(
      'operator' => 'not',
      'value' => '.thumbnail',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'case' => 1,
      'id' => 'filepath',
      'table' => 'files',
      'field' => 'filepath',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('use_ajax', TRUE);
  $handler->override_option('items_per_page', 15);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'list');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'type' => 'ul',
  ));
  $handler->override_option('row_plugin', 'imagebrowser_thumbnail');

  $views[$view->name] = $view;
  return $views;
}