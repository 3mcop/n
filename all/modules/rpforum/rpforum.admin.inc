<?php



function rpforum_admin_edit_forum_form($form_state, $type = "forum", $tid = -1) {
  $form = array();
 
  
  drupal_set_title("Add $type");
    
  $edit = array();  
  if ($tid > -1) {
    $temp = taxonomy_get_term($tid);
    $edit = array(
      'name' => $temp->name,
      'description' => $temp->description,
      'tid' => $temp->tid,
      'weight' => $temp->weight,
      'at_top' => variable_get("rpforum_at_top_$tid", ""),
    );
  } 
  
  $form['name'] = array('#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $edit['name'], 
    '#maxlength' => 255,
    '#description' => t('Short but meaningful name for this ' . $type . '.'),
    '#required' => TRUE,
  );
  $form['description'] = array('#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $edit['description'],
    '#description' => t("Description and guidelines for discussions within this $type."),
  );

  $form['at_top'] = array('#type' => 'textarea',
    '#title' => t('At the top of the page'),
    '#default_value' => $edit['at_top'],
    '#description' => t("When the user clicks on this $type, this text will appear at the top of the page.
                          It may contain any HTML.  For example, if you want certain rules to appear
                          for this $type, you may enter them here."),
  );
  
  
  
  $form['parent']['#tree'] = TRUE;
  $form['parent'][0] = rpforum_admin_forum_parent_select($edit['tid'], t('Parent'), 'forum');
    
  $form['weight'] = array('#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $edit['weight'],
    '#description' => t('Forums are displayed in ascending order by weight (forums with equal weights are displayed alphabetically).'),
  );

  $form['sub_type'] = array(
    "#type" => "hidden",
    "#value" => $type,
  );
  
  $form['vid'] = array('#type' => 'hidden', '#value' => variable_get('rpforum_nav_vocabulary', ''));
  $form['submit' ] = array('#type' => 'submit', '#value' => t('Save'));
  if ($edit['tid']) {
    $form['delete'] = array(
      '#type' => 'submit', 
      '#value' => t('Delete'),
      '#attributes' => array("onClick" => "return confirm(\"Are you sure you wish to delete this?  All sub forums (if any) will also be deleted!\")"),
      );
    $form['tid'] = array('#type' => 'hidden', '#value' => $edit['tid']);
  }
  $form['#submit'][] = 'rpforum_admin_edit_forum_form_submit';
  
  
  return $form;
}

function rpforum_admin_forum_parent_select($tid, $title, $child_type) {
  // Taken from forum.admin.inc, line 255.
$parents = taxonomy_get_parents($tid);
  if ($parents) {
    $parent = array_shift($parents);
    $parent = $parent->tid;
  }
  else {
    $parent = 0;
  }

  $vid = variable_get('rpforum_nav_vocabulary', '');
  $children = taxonomy_get_tree($vid, $tid);

  // A term can't be the child of itself, nor of its children.
  foreach ($children as $child) {
    $exclude[] = $child->tid;
  }
  $exclude[] = $tid;

  $tree = taxonomy_get_tree($vid);
  $options[0] = '<'. t('root') .'>';
  if ($tree) {
    foreach ($tree as $term) {
      if (!in_array($term->tid, $exclude)) {
        $options[$term->tid] = str_repeat(' -- ', $term->depth) . $term->name;
      }
    }
  }
  if ($child_type == 'container') {
    $description = t('Containers are usually placed at the top (root) level, but may also be placed inside another container or forum.');
  }
  else if ($child_type == 'forum') {
    $description = t('Forums may be placed at the top (root) level, or inside another container or forum.');
  }

  return array('#type' => 'select', '#title' => $title, '#default_value' => $parent, '#options' => $options, '#description' => $description, '#required' => TRUE);
  
}


function rpforum_admin_edit_forum_form_submit($form, &$form_state) {

  if ($form_state["values"]["op"] == "Delete") {
    // We are trying to delete this term.
    taxonomy_del_term($form_state['values']['tid']);
    drupal_set_message(t('The forum %term and all sub-forums have been deleted.', array('%term' => $form_state['values']['name'])));
    watchdog('content', 'rpforum: deleted %term and all its sub-forums and associated posts.', array('%term' => $form_state['values']['name']));

    $form_state['redirect'] = 'admin/settings/rpforum/forums';
    return;    
  }  
  
  if ($form_state["values"]["sub_type"] == 'container') {
    $container = TRUE;
    $type = t('forum container');
  }
  else {
    $container = FALSE;
    $type = t('forum');
  }

  
  
  $status = taxonomy_save_term($form_state['values']);
  switch ($status) {
    case SAVED_NEW:
      if ($container) {
        $containers = variable_get('rpforum_containers', array());
        $containers[] = $form_state['values']['tid'];
        variable_set('rpforum_containers', $containers);
      }
      drupal_set_message(t('Created new @type %term.', array('%term' => $form_state['values']['name'], '@type' => $type)));
      break;
    case SAVED_UPDATED:
      drupal_set_message(t('The @type %term has been updated.', array('%term' => $form_state['values']['name'], '@type' => $type)));
      break;
  }
  
  
  $tid = $form_state['values']['tid'];
  $at_top = trim($form_state["values"]["at_top"]);
  variable_set("rpforum_at_top_$tid", $at_top);  
  
  
  $form_state['redirect'] = 'admin/settings/rpforum/forums';
}


/**
 * My only objective here is to re-order the containers based on their positions from
 * the drag-and-drop page (should be able to use their weights).
 *
 * @param unknown_type $form
 * @param unknown_type $form_state
 */
function rpforum_admin_forums_reorder_containers_submit($form, $form_state) {
  
  $containers = variable_get('rpforum_containers', array());
  
  if (count($containers) == 0) return;
  
  // I am going to find out the weights of each container, then put them into
  // a new where I can sort based on those weights, then transfer them BACK to
  // my variable table in the correct order.
  
  $temp_array = array();
  foreach($containers as $tid) {
    $term = taxonomy_get_term($tid);
    $w = $term->weight;
    
    // Left-pad $w with lots of zeros so it will sort correctly.
    $w = str_pad((int) $w, 10, "0", STR_PAD_LEFT);
    
    // Add to our temp array.
    $temp_array[] = "$w~~$tid";
  }
  
  // Now, sort the temp_array.
  sort($temp_array);

  // Now, re-assemble back into our containers array.
  $containers = array();
  foreach ($temp_array as $line) {
    $temp = explode("~~", $line);
    $containers[] = $temp[1];  // the original tid.
  }
  
  // We can now set it back to the database!
  variable_set('rpforum_containers', $containers);
  drupal_set_message(t("Container and forum positions have been saved."));
   
}



function rpforum_admin_forums_form() {
  // code taken from forum.admin.inc line 271
  module_load_include('inc', 'taxonomy', 'taxonomy.admin');  

  $menu_base = variable_get("rpforum_menu_base", "rpforum");
  
  $vid = variable_get('rpforum_nav_vocabulary', '');
  $vocabulary = taxonomy_vocabulary_load($vid);
  $form = taxonomy_overview_terms($form_state, $vocabulary);
  
  foreach (element_children($form) as $key) {
    if (isset($form[$key]['#term'])) {
      $term = $form[$key]['#term'];
      $form[$key]['view']['#value'] = l($term['name'], "$menu_base/" . $term['tid']);
      if (in_array($form[$key]['#term']['tid'], variable_get('rpforum_containers', array()))) {
        $form[$key]['edit']['#value'] = l(t('edit container'), 'admin/settings/rpforum/edit/container/'. $term['tid']);
      }
      else {
        $form[$key]['edit']['#value'] = l(t('edit forum'), 'admin/settings/rpforum/edit/forum/'. $term['tid']);
      }
    }
  }

  // Remove the alphabetical reset.
  unset($form['reset_alphabetical']);

  // The form needs to have submit and validate handlers set explicitly.
  $form['#theme'] = 'taxonomy_overview_terms';
  
  $form['#submit'] = array('taxonomy_overview_terms_submit'); // Use the existing taxonomy overview submit handler.
  // Also add in a submit handler which will re-order the containers on the page!  Otherwise they will
  // only stay in the order they were added.
  $form['#submit'][] = 'rpforum_admin_forums_reorder_containers_submit';
  
  $form['#validate'] = array('taxonomy_overview_terms_validate');
  $form['#empty_text'] = '<em>'. t('There are no existing containers or forums. Containers and forums may be added using the <a href="@container">add container</a> and <a href="@forum">add forum</a> pages.', array('@container' => url('admin/settings/rpforum/add/container'), '@forum' => url('admin/settings/rpforum/add/forum'))) .'</em>';
  
  
  $form["markup_add_links"] = array(
    "#value" => "<div class='rpforum-admin-add-links'>
            " . l(t("Add container"), "admin/settings/rpforum/add/container") . " -
            " . l(t("Add forum"), "admin/settings/rpforum/add/forum") . "
                </div>",
  );
  
  $form["markup_help"] = array(
    "#value" => "<div>
                  " . t("To set up a new RP Forum, first create a container, then create
                  a forum.  Next, drag the forum UNDER and TO THE RIGHT of the 
                  container.") . "
                  <br><br>
                  <h3>" . t("Important note about subforums") . "</h3>
                  " . t("You may only have at most 2 levels of forums.  In other words, you may
                  have Container -> forum -> subforum.  RP Forum will not recognize any further sub forums.") . "
                  <br><br><h3>" . t("Example") . "</h3>
                  <b>" . l(t("Click here to see a example image"), $GLOBALS["base_url"] . "/" . drupal_get_path("module", "rpforum") . "/style/images/rpforum-settings-forums-screen1.png", array("attributes" => array("target" => "_blank"))) . " 
                  </b> " . t("of what your screen should look like when finished.") . "
                </div>",
  );
  
  
  return $form;
}





function rpforum_admin_bans_form() {
  $form = array();
  
  $form["rpforum_ban_list"] = array(
    "#type" => "textarea",
    "#title" => t("Ban list"),
    "#rows" => 10,
    "#description" => t("Enter IP addresses of banned users
                      <strong>one per line!</strong>  You may
                      enter a comment about each IP after it
                      by first placing a <strong>#</strong> and then your comment.
                      You may have wildcards by using <strong>*. Only one wildcard
                      per line is allowed!</strong>
                      <br><br>For example:<blockquote>
                       123.123.123.5  &nbsp;&nbsp;# spam bot<br>
                       123.76.23.88   &nbsp;&nbsp;# this guy was a jerk.<br>
                       145.88.77.*    &nbsp;&nbsp;# I don't like these people<br>
                       192.11.*       &nbsp;&nbsp;# don't like these folks either.</blockquote>"),
    "#attributes" => array("wrap" => "off"),
    "#default_value" => variable_get("rpforum_ban_list", "1.2.3.4   # example 1\n1.2.3.*   # example 2"),                  
  );
  
  $form["rpforum_ban_message"] = array(
    "#type" => "textarea",
    "#title" => t("Ban message"), 
    "#rows" => 3,
    "#default_value" => variable_get("rpforum_ban_message", "I'm sorry, but you cannot post messages at this time."),
    "#description" => t("This is the message which will be displayed to banned users
                      when they attempt to post a message.  It is not recommended that you state they
                      are banned, as this might encourage the user to change IPs to circumvent the ban.
                      It is better to let them think the forum is simply having a technical problem."),
  );
  
  
  return system_settings_form($form);
}

function rpforum_admin_user_settings_form() {
  $form = array();
  
  drupal_add_css(drupal_get_path("module", "rpforum") . "/style/rpforum.css");
  
  $form["badges"] = array(
    "#type" => "fieldset",
    "#title" => t("Badges settings"),
    "#collapsible" => true,
    "#description" => t("Badges are applied to users based on the number
                      of posts they have made.  These are categorized
                      into 5 tiers.  Please enter the # of posts and titles
                      for each tier below."),
  );
  
  for ($t = 1; $t <= 5; $t++) {
    $form["badges"]["rpforum_badge_posts_$t"] = array(
      "#title" => t("Number of posts for tier") . " #$t",
      "#type" => "textfield",
      "#size" => 3,
      "#default_value" => variable_get("rpforum_badge_posts_$t", ($t - 1) * 75),      
    );
  
    $form["badges"]["rpforum_badge_title_$t"] = array(
      "#title" => t("Title for users at tier") . " #$t",
      "#type" => "textfield",
      "#size" => 30,
      "#default_value" => variable_get("rpforum_badge_title_$t", "Registered User"),      
    );
    
    
  }
  

  $form["profiles"] = array(
    "#type" => "fieldset",
    "#title" => t("Profile settings"),
    "#collapsible" => true,
  );
  
  $form["profiles"]["rpforum_prevent_all_avatars"] = array(
    "#type" => "checkbox",
    "#title" => t("Do not handle avatars through RP Forum."),
    "#description" => t("If checked, RP Forum will not handle any avatars at all,
                       and all form elements relating to them will be removed from the
                       User profile page provided by RP Forum.  You might want to
                       do this if you are using Author Pane or some other technique
                       to manage avatars.  If checked, you may ignore all
                       other questions in this section.
                  <br><strong>If you are not sure what to do, leave this box unchecked.</strong>"),
    "#default_value" => variable_get("rpforum_prevent_all_avatars", 0),
  );

  
  $form["profiles"]["rpforum_allow_avatar_uploads"] = array(
    "#type" => "checkbox",
    "#title" => t("Allow users to upload avatars"),
    "#default_value" => variable_get("rpforum_allow_avatar_uploads", 0),
    "#description" => t("If checked, visitors will be allowed to upload pictures
                      to your site for use as avatars.  <b>ImageAPI</b> module is
                      required to use this feature!"), 
  );
  
  $form["profiles"]["rpforum_avatar_upload_directory"] = array(
    "#type" => "textfield",
    "#title" => t("Avatar upload directory"),
    "#default_value" => variable_get("rpforum_avatar_upload_directory", "rpforum_avatars"),
    "#description" => t("Enter the directory name in your site's /files/ directory where
                      to store avatar uploads (ignore if you are not allowing avatar uploads).
                      <br>The directory must already exist and have propper permissions set.
                      <br>Do not enter preceeding or trailing slashes!"),
  );
  
  $form["profiles"]["rpforum_avatar_scale_width"] = array(
    "#type" => "textfield",
    "#title" => t("Avatar scale width"),
    "#size" => 3,
    "#default_value" => variable_get("rpforum_avatar_scale_width", 100),
    "#description" => t("Enter the pixel width to scale avatar uploads and linked
                      images to.  Enter only a numeric value here."),
  );
  
  
  $form["#validate"][] = "rpforum_admin_user_settings_form_validate";
  
  return system_settings_form($form);
}


function rpforum_admin_user_settings_form_validate($form, &$form_state) {
  // We want to make sure that with avatar uploads, they have the imageAPI module
  // installed.  Also, that the specified directory for uploads exists.
  
  if ($form_state["values"]["rpforum_allow_avatar_uploads"] == 1) {
    // Make sure imageapi has been installed!
    if (!function_exists("imageapi_menu")) {
      form_error($form["profiles"]["rpforum_allow_avatar_uploads"], t("It does not look like you have ImageAPI installed!  
            You must install this module in order to use avatar uploads.
            Don't forget to enable an imageapi toolkit as well (like GD)!"));
    }
    
    // Make sure the upload directory exists!
    if (!file_exists(file_directory_path() . "/" . $form_state["values"]["rpforum_avatar_upload_directory"])) {
      form_error($form["profiles"]["rpforum_avatar_upload_directory"], t("The specified avatar upload directory does not already exist in your site's /files/ directory,
                                                            or, it does not have adequate permissions set.  Please create the directory and try again."));
    }
    
  }
  
  
}


function rpforum_admin_access_form() {
  $form = array();
  
  $form["markup" . $m++] = array(
    "#value" => t("Use this page to add moderators to specific forums, as well as control
                 which users are allowed to view/edit on a per-forum basis.
                  <br><br>Note: Moderators will be able to
                          edit and delete posts & topics, make topics sticky
                          or locked, as well as split and move topics (so long
                          as they are also moderators of the forum they wish
                          to move the topic post to).
                          <br><br>
                          Don't forget that if you wish to give moderators the
                          ability to delete or edit anything, you must visit
                          the Drupal permissions page and enable the per-content type
                          permissions there.
                          "),
  );
  
  
  // What is the behavior when a user is not allowed access to a forum?
  $form["rpforum_excluded_forum_behavior"] = array(
    "#title" => t("When a forum is excluded for a user..."),
    "#type" => "radios",
    "#options" => array("hide" => t("Hide the forum on the front page"),
                        "display" => t("Display the forum on the front page, but disable the link."),
                        ),
    "#default_value" => variable_get('rpforum_excluded_forum_behavior', 'hide'),    
    "#description" => t("If you select to display the forum, but disable the link, then extra CSS 
                         classes will also be applied so you can theme these 
                         excluded forums and containers however you wish."),
  );
  
  
  // We need to get the roles of the system, for later use.
  $roles = user_roles();
  $role_options = array();
  foreach ($roles as $rid => $name) {
    $role_options[$rid] = t("Exclude view access for") . " <i>$name</i>";
  }  
  
  // Go through and find the names of each container and forum...
  $vid = variable_get('rpforum_nav_vocabulary', '');  
  $containers = variable_get("rpforum_containers", array());
  foreach ($containers as $tid) {
    $container = taxonomy_get_term($tid);
    if ($container->vid != $vid) continue;
    
    $form["rpforum_container_$tid"] = array(
      "#type" => "fieldset",
      "#title" => t("Container: ") . $container->name,
      "#collapsible" => TRUE,
    );
    
    $children = taxonomy_get_children($tid);
    foreach($children as $ctid => $forum) {    
           
      $form["rpforum_container_$tid"]["rpforum_forum_$ctid"] = array(
        "#type" => "fieldset",
        "#title" => $forum->name,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );     
       
      $form["rpforum_container_$tid"]["rpforum_forum_$ctid"]["rpforum_moderators_$ctid"] = array(
        "#type" => "textarea",
        "#title" => t("Moderators for forum ") . $forum->name,
        "#rows" => 2,        
        "#default_value" => variable_get("rpforum_moderators_$ctid", ""),
        "#description" => t("Enter the usernames of moderators for this forum,
                          <b>one per line</b>."),                          
      );
      
    
      $form["rpforum_container_$tid"]["rpforum_forum_$ctid"]["rpforum_exclude_view_$ctid"] = array(
        "#type" => "checkboxes",
        "#title" => t("Exclude these roles from viewing ") . $forum->name,
        "#options" => $role_options,
        "#default_value" => variable_get("rpforum_exclude_view_$ctid", array()),
        "#description" => t("This will override the general 'access rpforums' permission, which
                            must be set before any user will be able to view any forums."),                          
      );
     
      
      // Subforums?
      $subforums = taxonomy_get_children($ctid);
      if (count($subforums) > 0) {
        $form["rpforum_container_$tid"]["rpforum_forum_$ctid"]["markup1"] = array(
          "#type" => "markup",
          "#value" => "<div>" . t("Please note: moderators and permissions must also be set for <b>subforums</b>.  Subforums are
                        treated as individual forums, and will not inherit moderators or permissions from the
                        forum above them.") . "</div>",
        );
        
        $form["rpforum_container_$tid"]["rpforum_forum_$ctid"]["subforums"] = array(
          "#type" => "fieldset",
          "#title" => $forum->name . " " . t("subforums"),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
        );     
        
        
        foreach ($subforums as $sctid => $subforum) {
          $form["rpforum_container_$tid"]["rpforum_forum_$ctid"]["subforums"]["rpforum_forum_$sctid"] = array(
            "#type" => "fieldset",
            "#title" => $subforum->name,
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
          );     
           
          $form["rpforum_container_$tid"]["rpforum_forum_$ctid"]["subforums"]["rpforum_forum_$sctid"]["rpforum_moderators_$sctid"] = array(
            "#type" => "textarea",
            "#title" => t("Moderators for forum ") . $subforum->name,
            "#rows" => 2,        
            "#default_value" => variable_get("rpforum_moderators_$sctid", ""),
            "#description" => t("Enter the usernames of moderators for this forum,
                              <b>one per line</b>."),                          
          );
          
        
          $form["rpforum_container_$tid"]["rpforum_forum_$ctid"]["subforums"]["rpforum_forum_$sctid"]["rpforum_exclude_view_$sctid"] = array(
            "#type" => "checkboxes",
            "#title" => t("Exclude these roles from viewing ") . $subforum->name,
            "#options" => $role_options,
            "#default_value" => variable_get("rpforum_exclude_view_$sctid", array()),
            "#description" => t("This will override the general 'access rpforums' permission, which
                                must be set before any user will be able to view any forums."),                          
          );
         
                    
        }
        
        
      } // if count > 0
       
    }
  }
     
  return system_settings_form($form);
}

function rpforum_admin_access_form_validate($form, $form_state) {
  
/*  foreach($form_state["values"] as $key => $value) {
    if (strstr($key, "rpforum_container_")) {
      foreach ($value as $key2 => $value2) {
        if (strstr($key2, "rpforum_moderators_")) {
          // Make sure the users listed are real users.
          $temp = explode("\n", $value2);
          foreach ($temp as $uname) {
            $uname = trim($uname);
            if ($uname == "") continue;
            if (!$tuser = user_load($uname)) {
              form_error($form[$key][$key2], "Sorry, but the username <b>$uname</b> could not be found.
                                       This user must already exist in the system before they
                                       can be assigned as a moderator.");
            }
            
            
          }
        }
      }
    }
  }
*/  

}


function rpforum_admin_form() {
  $form = array();
  
  // Are we running the latest version?  If not then display a message here.
  // Let's try to figure out the version number of this module.
  $version_code = $GLOBALS["rpforum_version_code"];
  
  $latest_version = trim(variable_get("rp_inst_latest_version_rpforum", ""));  
  $latest_version_code = trim(variable_get("rp_inst_latest_version_code_rpforum", "0"));
  
  if (variable_get("rp_inst_status_update", 1) == 1) {
    if ($version_code > 0 && $latest_version_code > 0 && $version_code < $latest_version_code) {
      drupal_set_message(t("You are not running the latest recommended version of RP Forum") . " (v$latest_version)." .
                          t("If you wish to upgrade, please visit 
                          <a href='http://www.richardpeacock.com/get-rpforum'>http://www.richardpeacock.com/get-rpforum</a>
                          to download the latest version.
                          If you have already upgraded and are still seeing this message, remember to run update.php
                          from the Modules page."), "warning");                        
    }
  }
  
  
  
  $form["general"] = array(
    "#type" => "fieldset",
    "#title" => t("General settings"),
    "#collapsible" => true,
  );
  $form["general"]["rpforum_title"] = array(
    "#title" => t("Forum title"),
    "#type" => "textfield",
    "#default_value" => variable_get("rpforum_title", "RP Forum"),
    "#description" => t("Enter a title for your forum.  This will appear in
                      the title bar as well as breadcrumbs."),
  );
  
  $form["general"]["rpforum_menu_base"] = array(
    "#title" => t("Menu base"),
    "#type" => "textfield",
    "#default_value" => variable_get("rpforum_menu_base", "rpforum"),
    "#description" => t("Enter the menu base for your forum.  For example,
                      if you want it at www.example.com/myforum, then
                      enter <strong>myforum</strong> in the box.  <strong>DO NOT</strong>
                      enter a preceeding or trailing slash!"), 
  );
  
  $form["general"]["rpforum_set_frontpage"] = array(
    "#title" => t("Set RP Forum to be default front page."),
    "#type" => "checkbox",
    "#options" => array("no", "yes"),
    "#description" => t("Check this box and click save if you would like your
                      RP Forum to be the default front page.  If it is already
                      the front page, you do not need to check this box.  You 
                      can <strong>unset or set a different front page</strong>
                      by going to admin/settings/site-information.
                      <br>
                      Current front page:") . " <strong>" . variable_get("site_frontpage", "") . "</strong>",
  );
  
  $form["general"]["rp_forum_front_public"] = array(
    "#title" => t("Make RP Forum's main (front) page publicly visible?"),
    "#type" => "checkbox",
    "#default_value" => variable_get("rp_forum_front_public", 1),
    "#description" => t("If checked, the main page of RP Forum may be viewed by anyone,
                       even those without the 'access rpforums' permission.
                       If unsure, leave this box checked."),
                       
  );
  
  /////////////////////////////////////////////////////////////////
  
  $form["editing"] = array(
    "#type" => "fieldset",
    "#title" => t("Editing"),
    "#collapsible" => true,
  );
  
  $form["editing"]["rpforum_use_bbcode"] = array(
    "#type" => "checkbox",
    "#title" => t("Use built-in BBcode buttons and filtering."),
    "#description" => t("Uncheck this if you are using another editor
                (like TinyMCE or fckeditor) or filter.
                <br><strong>If you are not sure what to do, leave this box checked.</strong>"),
    "#default_value" => variable_get("rpforum_use_bbcode", 1),
  );
  
  $form["editing"]["rpforum_use_smileys"] = array(
    "#type" => "checkbox",
    "#title" => t("Use built-in smileys."),
    "#description" => t("Uncheck this if you are using another smiley module/filter.
                <br><strong>If you are not sure what to do, leave this box checked.</strong>"),
    "#default_value" => variable_get("rpforum_use_smileys", 1),
  );
  
  
  $form["editing"]["rpforum_filter_words"] = array(
    "#type" => "textarea",
    "#title" => t("Filter out bad words"),
    "#description" => t("Use this box to filter out bad words, or any
                         strings you do not want to appear.  Enter them
                         one per line, with a pipe character (|) between
                         the original word and its replacement.  Ex:
                         <br>
                         original|*******<br>
                         poop|p**p<br>
                         telephone|t**ephone"),
    "#default_value" => variable_get("rpforum_filter_words", ""),
  );
  
  
  ////////////////////////////////////////////////////////////////
  
  
  $form["search"] = array(
    "#type" => "fieldset",
    "#title" => t("Search and Log in"),
    "#collapsible" => true,
  );
  $form["search"]["rpforum_search_block_title"] = array(
    "#type" => "textfield",
    "#title" => t("Search block title"),
    "#default_value" => variable_get("rpforum_search_block_title", "Search the forums"),
    "#description" => t("This text will appear above the search textfield in the RP Forum Search
                      block (which you can enable on sitename.com/admin/build/block, if desired)."),
  );
  $form["search"]["rpforum_enable_search_field"] = array(
    "#type" => "checkbox",
    "#title" => t("Enable the search field on forum pages"),
    "#default_value" => variable_get("rpforum_enable_search_field", 1),
    "#description" => t("If checked, this will automatically display the search box and button on forum
                      pages.  If you are planning to use the RP Forum Search block, then you should
                      probably disable this by unchecking it.
                      <br><b>If you are not sure what to do, leave this box checked.</b>"),
  );

  $form["search"]["rpforum_enable_login_account_field"] = array(
    "#type" => "checkbox",
    "#title" => t("Enable the login or account fields on forum pages"),
    "#default_value" => variable_get("rpforum_enable_login_account_field", 1),
    "#description" => t("If checked, this will automatically display the login boxes on forum pages,
                        next to where the search field would be if you enable it.  When a user is
                        logged in, this area shows a link to their account page and a log out link.
                      <br><b>If you are not sure what to do, leave this box checked.</b>"),
  );
  
  
  
  
  ////////////////////////////////////////////////////////////////
  
  
  $form["feed"] = array(
    "#type" => "fieldset",
    "#title" => t("Feed"),
    "#collapsible" => true,
  );
  $form["feed"]["rpforum_feed_title"] = array(
    "#type" => "textfield",
    "#title" => t("Forum feed title"),
    "#default_value" => variable_get("rpforum_feed_title", "Recent forum posts"),
    "#description" => t("This is the title of your forum's feed.  If a user subscribes
                      to posts from your forum (say, in Google Reader), then this
                      is what the feed will be named."),
  );
  $form["feed"]["rpforum_enable_feed_icon"] = array(
    "#type" => "checkbox",
    "#title" => t("Enable the feed icon at the bottom of the main page"),
    "#default_value" => variable_get("rpforum_enable_feed_icon", 1),
    "#description" => t("If checked, this will place an icon to the forum's feed
                      at the bottom of the main page.  You might uncheck this
                      if you do not want visitors to know about the feed's
                      existence.
                      <br><b>If you are not sure what to do, leave this box checked.</b>"),
  );
  
      
  ////////////////////////////////////////////
  $form["display"] = array(
    "#type" => "fieldset",
    "#title" => t("Display"),
    "#collapsible" => true,
  );

  $form["display"]["rpforum_container_name_is_link"] = array(
    "#type" => "checkbox",
    "#title" => t("Container names are links on the front page."),
    "#description" => t("If checked, this will display the container names on the front page
                          as links that, when clicked, take the visitor to a sub-page displaying
                          only information and forums for that container.
                  <br><strong>If you are not sure what to do, leave this box checked.</strong>"),
    "#default_value" => variable_get("rpforum_container_name_is_link", 1),
  );
  
  
  $form["display"]["rpforum_display_pager_at_top"] = array(
    "#type" => "checkbox",
    "#title" => t("Display pager links at top as well as bottom of pages."),
    "#description" => t("If checked, this will display the pager links at both the top
                    and the bottom of the page, when there are multiple pages.
                  <br><strong>If you are not sure what to do, leave this box checked.</strong>"),
    "#default_value" => variable_get("rpforum_display_pager_at_top", 1),
  );
  
  
  $form["display"]["rpforum_use_author_pane"] = array(
    "#type" => "checkbox",
    "#title" => t("Use the Author Pane module to display user information on posts."),
    "#description" => t("Requires you download and install the Author Pane module, as well
                  as any others you wish to use with it (like User Badges, User Stats, etc).
                  If you do not check this box, then rpforum will provide its own scaled-down
                  version of what those other modules can provide.
                  <br><strong>If you are not sure what to do, leave this box unchecked.</strong>"),
    "#default_value" => variable_get("rpforum_use_author_pane", 0),
  );
  
  
  $form["display"]["rpforum_hot_count"] = array(
    "#type" => "textfield",
    "#title" => t("Topic hot count"),
    "#size" => 3,
    "#default_value" => variable_get("rpforum_hot_count", 25),
    "#description" => t("Display a \"hot\" icon next to topics with this many or more
                      <strong>replies</strong>.  <br><strong>If you are not sure what to do, leave this box at 25.</strong>"),
  );
  
  $options = array(
    "small" => t("short"),
    "medium" => t("medium"),
    "large" => t("long"),
  );
  
  $form["display"]["rpforum_selected_date_format"] = array(
    "#type" => "select",
    "#title" => t("Date format of posts"),
    "#options" => $options,
    "#default_value" => variable_get("rpforum_selected_date_format", "medium"),
    "#description" => t("This is how dates of posts will be displayed in the forum.
                        The list is taken from your site's list of available Date
                        and Time formats, which is ") . l(t("configured here"), "admin/settings/date-time") . ".
                        <br>
                        <b>" . t("If unsure what to select, select 'medium'.") . "</b>",
  );

  $form["display"]["rpforum_wrap_length"] = array(
    "#type" => "textfield",
    "#title" => t("Body word-wrap length"),
    "#size" => 3,
    "#default_value" => variable_get("rpforum_wrap_length", 75),
    "#description" => t("Attempt to word-wrap every X characters in post bodies using a newline character. 
                          This prevents your theme from
                          breaking if someone posts a long URL.  Enter 0 to disable.
                        <br><strong>If you are not sure what to do, leave this box at 75.</strong>"),
  );
  
  
  ////////////////////////////////////
  
  $form["abuse"] = array(
    "#type" => "fieldset",
    "#title" => t("Report abuse"),
    "#collapsible" => true,
  );

  $form["abuse"]["rpforum_enable_report_abuse"] = array(
    "#type" => "checkbox",
    "#title" => t("Enable 'Report abuse' link on posts."),
    "#description" => t("If checked, this will give users with the correct permission
                       a link at the bottom of posts to be able to report abuse
                       of the system. <b>If you enable this, do not forget to
                       give users permission to use it on the permissions page!</b>
                  <br>If you are not sure what to do, leave this box unchecked."),
    "#default_value" => variable_get("rpforum_enable_report_abuse", 0),
  );

  $form["abuse"]["rpforum_report_abuse_tomail"] = array(
    "#type" => "textfield",
    "#title" => t("Send abuse reports to (email address)"),
    "#description" => t("Enter an email address abuse reports should be sent to."),
    "#default_value" => variable_get("rpforum_report_abuse_tomail", "example@example.com"),
  );
  
  $form["abuse"]["rpforum_min_time_between_reports"] = array(
    "#type" => "textfield",
    "#size" => "10",
    "#title" => t("Minimum number of minutes between reports"),
    "#description" => t("This is the number of minutes a user must wait after submitting
                       a report before they can submit another.  This value helps
                       deter malicious spam attacks.
                       <br><strong>If unsure what to do, leave this box set to 5</strong>"),
    "#default_value" => variable_get("rpforum_min_time_between_reports", "5"),
  );
  
  //////////////////////////////////////  
  
    
  $form["advanced"] = array(
    "#type" => "fieldset",
    "#title" => t("Advanced settings"),
    "#collapsible" => true,
  );
  $form["advanced"]["rpforum_advanced_rebuild_node_access"] = array(
    "#type" => "checkbox",
    "#title" => t("Rebuild the node_access table permissions on every save and view."),
    "#default_value" => variable_get("rpforum_advanced_rebuild_node_access", 0),
    "#description" => t("Some users have reported issues with using RP Forum with
                          other modules like Simple Access or Content Permissions.
                          Their solution was to 'Rebuild permissions' after every
                          save and before every view.  
                          If you check this box, this is basically what
                          it will do. <b>Be aware, this may represent a large performance decrease.</b>
                          <br>If unsure what to do, leave this box unchecked."),
  );  
  
  
  
  //////////////////////////////////////
  
  $form["rp_inst_status"] = array(
    "#type" => "fieldset",
    "#collapsible" => TRUE,
    "#collapsed" => TRUE,
    "#title" => t("Check for new versions"),
  );
  $form["rp_inst_status"]["rp_inst_status_update"] = array(
    "#type" => "checkbox",
    "#title" => t("Allow checking for new versions with richardpeacock.com?"),
    "#default_value" => variable_get("rp_inst_status_update", 1),
    "#description" => t("If enabled, this module will check with richardpeacock.com 
                      for new, recommended versions every few days.  If an update is found,
                      a message will be displayed at the top of this page.
                      No usuage, user, personal, or IP data about your site is sent;
                      only the installation details and module version are transmitted.  Uncheck
                      to disable.
                      <br><strong>If unsure what to do, leave this box checked.</strong>"),
  );
  
  
  
  $form["#submit"][] = "rpforum_admin_form_submit";
  
  return system_settings_form($form);
}





function rpforum_admin_form_submit($form, $form_state) {
  // We want to rebuild the menu router when this form is submitted!

  if ($form_state["clicked_button"]["#id"] == "edit-reset")
  {
    // User is trying to reset.  So, let's just reset the menu_base value.
    $form_state["values"]["rpforum_menu_base"] = "rpforum";
    $form_state["values"]["rpforum_title"] = "RP Forum";
  }

  // We have to do this before the menu rebuild, or it will not be in there yet.
  variable_set("rpforum_menu_base", $form_state["values"]["rpforum_menu_base"]);
  variable_set("rpforum_title", $form_state["values"]["rpforum_title"]);
  
  if ($form_state["values"]["rpforum_set_frontpage"] == 1) {
    // The user wants this to be their default front page.
    variable_set("site_frontpage", $form_state["values"]["rpforum_menu_base"]);
  }
  
  menu_rebuild();

}

function rpforum_admin_tools() {
  $rtn = "";

  $action = $_REQUEST["action"];
  if ($action == "rebuild_replies_count") {
    rpforum_rebuild_topic_reply_counts();
  }
  
  if ($action == "repair_post_taxonomy") {
    rpforum_repair_post_taxonomy();
  }
  
  $rtn .= t("Select an action from the list below.") . "
  
    <ul>
      <li><b>" . l(t("Rebuild topic relies counts"), "admin/settings/rpforum/tools", array("query"=>"action=rebuild_replies_count")) . "</b></li>
      <li><b>" . l(t("Repair missing or mismatched forum post taxonomy"), "admin/settings/rpforum/tools", array("query"=>"action=repair_post_taxonomy", "attributes" => array("onClick" => "return confirm(\"" . t("Are you sure you wish to repair missing or mismatched forum post taxonomy? Make sure you have backed up your database first!") . "\");"))) . "</b>
            - " . t("This tool will examine all the topic posts in the system, then go through every
                     post and reassign its taxonomy to the same taxonomy as its topic post.
                     This can be an exhaustive processes, and if you have a great many posts, it might
                     timeout.  <strong>Backup your database before proceeding!</strong>") . "</li>
    </ul>
  ";
  
  
  return $rtn;
}


?>