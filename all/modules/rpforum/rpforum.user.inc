<?php

function rpforum_build_user_profile($uid) {
  $res = db_result(db_query("SELECT profile_data FROM {rpforum_user_profiles}
                             WHERE uid = %d", $uid));
  if ($x = unserialize($res)) {
    $x["uid"] = $uid;
    return $x;
  }
  else {
    return array();
  }
}

function rpforum_get_user_subscriptions($uid) {
  $rtn = array();
  $res = db_query("SELECT * FROM {rpforum_user_subscriptions}
                  WHERE uid = %d
                  ", $uid);
  while ($cur = db_fetch_array($res)) {
    $rtn[] = $cur["topic_nid"];
  }
   
  return $rtn; 
  
}


function rpforum_user_profile_form($form_state, $the_user) {
  global $user;
  $uid = $the_user->uid;
  $menu_base = variable_get("rpforum_menu_base", "rpforum");
  
  if (($user->uid != $uid && !user_access("administer rpforums")) || ($uid < 1) ) {
    drupal_set_message(t("You do not have access to edit that profile."));
    drupal_goto("<front>");
    return array();
  }
  
  drupal_add_css(drupal_get_path("module", "rpforum") . "/style/rpforum.css");
  
  $form = array();
  $form['#attributes'] = array('enctype' => "multipart/form-data");
  
  $user_profile = rpforum_build_user_profile($uid);
  
  $form["the_uid"] = array(
    "#type" => "hidden",
    "#value" => $uid,
  );
  
  $form["avatar"] = array(
    "#type" => "fieldset",
    "#title" => t("Avatar"),
    "#collapsible" => true,
  );
  
  
  if (variable_get("rpforum_allow_avatar_uploads", 0) == 1) {
  
    if ($user_profile["avatar_image"]["thumb"] != "") {
      
      $img_directory = $GLOBALS["base_url"] . "/" . file_directory_path() . "/" . variable_get("rpforum_avatar_upload_directory", "rpforum_avatars");
      $form["avatar"]["avatar_image_markup"] = array(
        "#value" => "<div>" . t("Currently uploaded avatar image:") . "
                    <img src='$img_directory/{$user_profile["avatar_image"]["thumb"]}'>
                      </div>",
        
      );
    }
    
    $form["avatar"]["avatar_image"] = array(
      "#type" => "file",
      "#title" => t("Upload an avatar image"),
      "#description" => t("To remove your avatar image entirely, enter \"none\"
                        in the box below (the URL box)."),
    );
  
  }
  else {
    $gravuploadmessage = "<br>" . t("If you need a place to upload an avatar image, consider using ") . "
                      <a href='http://www.gravatar.com' target='_blank'>gravatar.com</a>.";
  }
  
  $thumb_width = variable_get("rpforum_avatar_scale_width", 100);
  $desc =   t("You may specify a url to an avatar image here.
                      Images will be scaled to @wx@w when displayed.
                      <br>Enter the full URL, including http:// ", array("@w" => $thumb_width)) . "
                      $gravuploadmessage";
  if (variable_get("rpforum_allow_avatar_uploads", 0) == 1) {
    $desc .= "<br><b>" . t("If any value is present here, the uploaded
                      image will be ignored!") . "</b>";
  }  
  $form["avatar"]["avatar_url"] = array(
    "#type" => "textfield",
    "#title" => t("Avatar image URL"),
    "#description" => $desc,
    "#default_value" => $user_profile["avatar_url"],
  );
  
  if (variable_get("rpforum_prevent_all_avatars", 0) == 1) {
    // Actually, no avatar information, please.
    unset($form["avatar"]);
  }
  
  
  $form["personal"] = array(
    "#type" => "fieldset",
    "#title" => t("Personal information"),
    "#description" => "<strong>" . t("All personal information is optional.") . "</strong>",
    "#collapsible" => true,
  );
  
  $form["personal"]["real_name"] = array(
    "#type" => "textfield",
    "#title" => t("Real name"),
    "#default_value" => $user_profile["real_name"],
  );
  
  $form["personal"]["email_enable"] = array(
    "#type" => "checkbox",
    "#title" => t("Make email address public"),
    "#default_value" => $user_profile["email_enable"],
    "#description" => "<b>" . t("Email:") . " $user->mail</b><br>
                        " . t("If checked, other users will be able to 
                      discover your email address and email you."),
    
  );
  
  $form["personal"]["website"] = array(
    "#type" => "textfield",
    "#title" => t("Website"),
    "#default_value" => $user_profile["website"],
    "#description" => t("Please enter full url, including http://"),
  );
  
  
  $form["personal"]["signature"] = array(
    "#type" => "textarea",
    "#title" => t("Signature"),
    "#rows" => 3,
    "#default_value" => $user_profile["signature"],
    "#description" => t("You may use BBCode syntax here. Ex: [b][/b], 
                        [url][/url], etc.  HTML is not allowed."),
  );
  
  /////////////////////////////
  $form["subscriptions"] = array(
    "#type" => "fieldset",
    "#title" => t("Subscriptions"),
    "#collapsible" => true,
  );

  $form["subscriptions"]["auto_subscribe"] = array(
    "#type" => "checkbox",
    "#title" => t("Auto-subscribe to all topics in which I write a post."),
    "#description" => t("If checked, this means you will, by default, be notified
                      by email of new posts in topics in which you write something."),
    "#default_value" => $user_profile["auto_subscribe"],
                      
  );
  
  $form["subscriptions"]["list"] = array(
    "#type" => "fieldset",
    "#title" => t("List of my subscriptions (click to expand/collapse)"),
    "#collapsed" => true,
    "#collapsible" => true,
    "#description" => t("This is a list of topics you have previously
                    subscribed to.  You can un-subscribe by simply
                    un-checking the checkbox next to the topic, and clicking Save."),
    "#attributes" => array("class" => "list-subscriptions"),
  );
  
  $subscriptions = rpforum_get_user_subscriptions($uid);
  foreach($subscriptions as $topic_nid) {
    $sub_node = node_load($topic_nid);
    if ($sub_node) {
      $username = $sub_node->name;
      if ($username == "") {
        $username = $sub_node->field_anon_name[0]["value"];
        
      }
      $tid = key($sub_node->taxonomy);
      $sub_title = l("$sub_node->title", "$menu_base/$tid/$topic_nid", array("attributes" => array("target"=>"blank"))) . " 
                    started by $username";
      $form["subscriptions"]["list"]["sub_$topic_nid"] = array(
        "#type" => "checkbox",
        "#title" => "",
        "#default_value" => 1,
        "#prefix" => "<div>",
        "#suffix" => "<span>$sub_title</span>
                       </div>",
      );
    }
  }
  
  
  
  $form["submit"] = array(
    "#type" => "submit",
    "#value" => t("Submit"),
  );
  
  
  $form["#redirect"] = "user/$uid";
  $form["#submit"][] = "rpforum_user_profile_form_submit";
  
  return $form;
  
}


function rpforum_user_profile_form_submit($form, $form_state) {
  // Let's actually insert our data into our database table.  
  // (we might also be uploading a picture here)
  global $user;
  
  $p = array();
  
  $uid = $form_state["values"]["the_uid"];
  $p = rpforum_build_user_profile($uid);  // begin by loading the original profile.
  $p["avatar_url"] = trim(check_plain($form_state["values"]["avatar_url"]));
  $p["signature"] = trim(check_plain($form_state["values"]["signature"]));
  $p["real_name"] = trim(check_plain($form_state["values"]["real_name"]));
  $p["website"] = trim(check_plain($form_state["values"]["website"]));
  $p["email_enable"] = trim($form_state["values"]["email_enable"]);
  $p["auto_subscribe"] = trim($form_state["values"]["auto_subscribe"]);
  
  ////////////////////////////////////////////////
  ////  Handle subscriptions
  ////////////////////////////////////////////////
  // Easiest way to do this is delete everything for this
  // user, then loop through and add 'em back in.
  db_query("DELETE FROM {rpforum_user_subscriptions}
            WHERE uid = %d", $uid);
  foreach($form_state["values"] as $key => $value) {
    if (substr($key, 0, 4) == "sub_" && $value == 1) {
      $topic_nid = str_replace("sub_", "", $key);
      db_query("INSERT INTO {rpforum_user_subscriptions}
                (uid, topic_nid) VALUES (%d, %d) ", $uid, $topic_nid);
    }
  }
  
  
  ////////////////////////////////////////////////
  ////  Handle avatar image upload
  ///////////////////////////////////////////////
  $output_directory = file_directory_path() . "/" . variable_get("rpforum_avatar_upload_directory", "rpforum_avatars");
  $thumb_width = variable_get("rpforum_avatar_scale_width", 100);
  
  if ($file = file_save_upload("avatar_image", array(), $output_directory)) {
    // We have saved the file OK.
    drupal_set_message(t("Uploading file ") . "$file->filename.");
    file_set_status($file, 1); // make it permanant

    // Get the actual filename from the filepath string.  Necessary because
    // if you upload 2 with the same name, it gets renamed to have a _0 after it,
    // but this is NOT reflected in $file->filename !
    $filename = str_replace($output_directory . "/", "", $file->filepath);


    // We now want to create a thumbnail
    // version of it.

    $thumb_filename = "thumb_" . $filename;
    if (!$thumb_image = imageapi_image_open($file->filepath)) {
      drupal_set_message(t("Could not open file @file.  Perhaps
                          there are no image toolkits enabled?", array("@file" => $file->filepath)), "error");
      return;
    }

    imageapi_image_scale($thumb_image, $thumb_width, NULL, false);

    if (!imageapi_image_close($thumb_image, $output_directory . "/" . $thumb_filename)) {
      drupal_set_message(t("Could not save file: ") . "$output_directory / $thumb_filename", "error");
    }

    // Add the thumb file to the files table.  The original is already there.
    db_query("INSERT INTO {files} (uid, filepath, status)
              VALUES (%d, '%s', '1')", $uid, $output_directory . "/$thumb_filename");

    // Note in our $p array what its information is.
    $p["avatar_image"]["original"] = $filename;
    $p["avatar_image"]["thumb"] = $thumb_filename;
    $bool_uploaded_file = true;
  }
  
  
  //////////////////////////////////////////////////

  
  
  // if they entered anything as the avatar_url, then we should delete
  // their old picture.  Also, if they uploaded a new picture.
  if ($p["avatar_url"] == "none" || $bool_uploaded_file == true) {
    $old_profile = rpforum_build_user_profile($uid);
    db_query("UPDATE {files}
             SET status = 0, timestamp = 0
             WHERE filepath = '%s'
             OR filepath = '%s' ", 
              $output_directory . "/" . $old_profile["avatar_image"]["thumb"],      
              $output_directory . "/" . $old_profile["avatar_image"]["original"]);
  }
    
  // If avatar_url == "none" then just clear it.
  if ($p["avatar_url"] == "none") {
    $p["avatar_url"] = "";
    $p["avatar_image"] = array();
  }
  
  // Okay, let's insert into the table!
  $serial_p = serialize($p);  
  db_query("UPDATE {rpforum_user_profiles}
            SET profile_data = '%s'
            WHERE uid = %d ", $serial_p, $uid);
  if (db_affected_rows() == 0) {
    db_query("INSERT INTO {rpforum_user_profiles}
            (profile_data, uid)
            VALUES ('%s', %d) ", $serial_p, $uid);           
  }
  drupal_set_message("User profile updated.");
  
  
}


function rpforum_user($op, &$edit, &$account, $category) {
  global $user;
  
  
  if ($op == "view")
  {
    drupal_add_css(drupal_get_path("module", "rpforum") . "/style/rpforum.css");
   
    // Should we add the privatemsg link?
    if (function_exists("privatemsg_list") && user_access("read privatemsg")) {
      $account->content["rpforum_privatemsg"] = array(
        "#title" => t("Private Messages"),
        "#type" => "user_profile_category",
        "#weight" => 2,
      );
      if ($user->uid == $account->uid) {
        $account->content["rpforum_privatemsg"]["read_private"] = array(
          "#title" => "",
          "#type" => "user_profile_item",
          "#value" => l(t("Read private messages"), "messages"),
        );
        
      }
    }
    
    
   $user_profile = rpforum_build_user_profile($account->uid);
   
   $email_info = "- not shown -"; 
   if ($user_profile["email_enable"] == "1") {
     $email_info = $account->mail;
   }   
   
   $account->content["rpforum_profile_info"] = array(
    "#title" => t("Forum Profile Information"),
    "#type" => "user_profile_category",    
    
    "avatar" => array(
      "#title" => t("Avatar"),
      "#type" => "user_profile_item",
      "#value" => rpforum_render_user_avatar($user_profile),
      "#weight" => 1,
      ),    

    "real_name" => array(
      "#title" => t("Real Name"),
      "#type" => "user_profile_item",
      "#value" => $user_profile["real_name"],
      "#weight" => 2,
      ),    
    

    "email" => array(
      "#title" => t("Email"),
      "#type" => "user_profile_item",
      "#value" => $email_info,
      "#weight" => 3,
      ),    
        
    "website" => array(
      "#title" => t("Website"),
      "#type" => "user_profile_item",
      "#value" => $user_profile["website"],
      "#weight" => 4,
      ),          
      
    "signature" => array(
      "#title" => t("Signature"),
      "#type" => "user_profile_item",
      "#value" => rpforum_render_user_signature($user_profile),
      "#weight" => 5,
      ),    
   );
    unset($account->content["summary"]);
   
    if (variable_get("rpforum_prevent_all_avatars", 0) == 1) {
      // We do not want to show anything about the avatars.
      unset($account->content["rpforum_profile_info"]["avatar"]);
    }    
     
    
    // Should we add the edit-profile link?
    if ($user->uid == $account->uid && $user->uid != 0) {
      $account->content["edit_profile_link"] = array(
        "#value" => "<div class='rpforum-edit-profile-link'>" . l(t("Edit forum profile settings"), "user/$user->uid/edit-profile") . "</div>",
        "#weight" => -1,
        );      
    }
     
  }
  
}


function rpforum_user_register_validate($form, $form_state) {
  
  // Has the user's IP been banned?  If so, do not let them register!
  if (rpforum_ip_is_banned()) {
    form_error($form["name"], t("Sorry, but registration is blocked at this time."));
    $bad_ip = $_SERVER['REMOTE_ADDR'];
    watchdog("rpforum", "User registration blocked by ip: $bad_ip. Attempted name: {$form_state["values"]["name"]} ", array(), WATCHDOG_ALERT);
  }
  
}

function rpforum_render_author_links($user_profile) {
  // This function will render links for each author (that link to their
  // profile, their email, and their website, if entered.
  global $user;
  
  $author_user = user_load($user_profile["uid"]);
  
  $img_path = $GLOBALS["base_url"] . "/" . drupal_get_path("module", "rpforum") . "/style/images/";
  
  $rtn = "";

  if (user_access("access user profiles")) {
    $rtn .= l("<img src='$img_path/profile_sm.gif' title='" . t("View Profile") . "'> ", "user/" . $user_profile["uid"], array("html"=>true, "attributes" => array("class" => "view-profile")));
  }
  
  if ($user_profile["website"] != "") {
    $rtn .= l("<img src='$img_path/www_sm.gif' title='" . t("Website") . "'> ", $user_profile["website"], array("html" => true, "attributes"=>array("target"=>"blank", "class" => "website")));
  }
  
  if ($user_profile["email_enable"] == "1") {
    $rtn .= l("<img src='$img_path/email_sm.gif' title='" . t("Send Email") . "'> ", "mailto: " . $author_user->mail, array("html"=>true, "attributes" => array("class" => "send-email")));
  } 
  
  // Is the privatemsg module installed?
  if (function_exists("privatemsg_menu")) {
    $rtn .= l("<img src='$img_path/im_off.gif' title='" . t("Send PM") . "'> ", "messages/new/" . $user_profile["uid"], array("html" => true, "query" => drupal_get_destination(), "attributes" => array("class" => "send-pm")));
    
  }
  
  
  
  return $rtn;
}


function rpforum_render_user_avatar($user_profile) {

  if (variable_get("rpforum_prevent_all_avatars", 0) == 1) {
    return "<div class='user-profile-avatar-no-image'> </div>";
  }
  
  $img_directory = $GLOBALS["base_url"] . "/" . file_directory_path() . "/" . variable_get("rpforum_avatar_upload_directory", "rpforum_avatars");
  $thumb_width = variable_get("rpforum_avatar_scale_width", 100);
  
  $img_url = "";
  
  // Something has been entered for the user's avatar URL.  Make sure it has http somewhere in it.
  if ($user_profile["avatar_url"] != "" && strstr($user_profile["avatar_url"], "http")) {
    $img_url = $user_profile["avatar_url"];
  }
  
  if ($user_profile["avatar_image"]["thumb"] != "") {
    $img_url = $img_directory . "/" . $user_profile["avatar_image"]["thumb"];
  }
  
  $width_property = " width='$thumb_width' ";
  if ($thumb_width == "") {
    $width_property = "";
  }
  
  if ($img_url != "") {
    $rtn .= "<div class='user-profile-avatar'>";
    $rtn .= "<img src='$img_url' $width_property class='user-profile-avatar-image'>";
    $rtn .= "</div>";
    return $rtn;
  }
  
  return "<div class='user-profile-avatar-no-image'> </div>";
  
}

function rpforum_render_user_signature($user_profile) {
  return "<div class='user-profile-signature'>" . rpforum_convert_bbcode_to_html($user_profile["signature"]) . "</div>";
}


function rpforum_render_login_or_account() {
  global $user;
  $rtn = "";

  $rtn .= "<div class='rpforum-login-info'>";
  if ($user->uid < 1) {
    // Not logged in yet.  Provide login boxes and a Create account link.
    /*
    $rtn .= '<form id="user-login" method="post" accept-charset="UTF-8" action="' . $GLOBALS["base_url"] . '/user">';
    $rtn .= "Login: <input type='text' name='name' size='10' class='form-text'> ";
    $rtn .= "<input type='password' name='pass' size='10' class='form-text'> ";
    $rtn .= "<input type='submit' value='Login' class='form-submit'>";
    $rtn .= '<input id="edit-user-login" type="hidden" value="user_login" name="form_id"/>';
    $rtn .= " or " . l("Create an account", "user/register") . "!
              </form>";
    $rtn .= "";
    */
    
  /*  $rtn .= "<table border='0' cellspacing='0' cellpadding='0'><td>Log in:</td><td>";
    $rtn .= drupal_get_form("user_login_block");
    $rtn .= "</td><td> or " . l("Create an account", "user/register");
    $rtn .= "</td></table>";
  */ 
  
    $rtn .= "<div style='float:right;'> &bull; " . l(t("New account"), "user/register") . "</div>";
  
    $rtn .= "<div style='float:right; padding-right: 5px;'>";
    $rtn .= drupal_get_form("user_login_block");
    $rtn .= "</div>";
    
    $rtn .= "<div style='float: right; padding-right: 5px;'>Log in:</div>";
    
    
    
   
  //$rtn .= "here";    
  } else {
    // user IS logged in.  Provide a link to their account page and logout link.
    $rtn .= t("Welcome ") . $user->name . "! ";
    $rtn .= l(t("My Account"), "user") . " - ";
    $rtn .= l(t("Logout"), "logout");
  }
  
  $rtn .= "</div>";
  
    
  return $rtn;
}


?>