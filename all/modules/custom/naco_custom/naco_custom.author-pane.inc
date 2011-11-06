<?php

/**
 * @file
 * Provides a preprocess function for the author pane used by
 * Advanced Forum and Advanced Profile Kit.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function naco_custom_preprocess_author_pane(&$variables) {
  $account_id = $variables['account']->uid;

  if ($account_id != 0) {

    // Load the post author object if not loaded already.
    static $author;
    if ( empty( $author ) || $author->uid != $account_id ) {
      $author = user_load( $account_id );
    }

    // If the author is a sponsor then make that visually obvious.
    if ( in_array( 'forum sponsor', $author->roles ) ) {
      $variables['account_name'] =
        '<h3>' . t( 'Forum Sponsor' ) . '</h3>' . $variables['account_name'];
    }
  }
}
