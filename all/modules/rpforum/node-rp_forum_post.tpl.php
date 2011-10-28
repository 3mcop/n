<?php

/**
 * @file node.tpl.php
 *
 * Theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: Node body or teaser depending on $teaser flag.
 * - $picture: The authors picture of the node output from
 *   theme_user_picture().
 * - $date: Formatted creation date (use $created to reformat with
 *   format_date()).
 * - $links: Themed links like "Read more", "Add new comment", etc. output
 *   from theme_links().
 * - $name: Themed username of node author output from theme_user().
 * - $node_url: Direct url of the current node.
 * - $terms: the themed list of taxonomy term links output from theme_links().
 * - $submitted: themed submission information output from
 *   theme_node_submitted().
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables: 
 * - $teaser: Flag for the teaser state.
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 */

$pC = "";

if ($node->force_zebra != "") {
  $zebra = $node->force_zebra;
}

$message_icon_path = $GLOBALS["base_url"] . "/" . drupal_get_path("module", "rpforum") . "/icons/default";

$quote_body = $node->field_body2[0]["value"];
// filter out naughty words from quote_body, if there are any to filter.
$quote_body = rpforum_filter_bad_words($quote_body);

$plain_name = $node->name;

$author_type = t("Registered User");
if ($node->uid == 0) {
  if ($node->field_anon_name[0]["value"] != "") {
    $name = check_plain($node->field_anon_name[0]["value"]);
    $plain_name = $name;
   }
  $author_type = t("Guest");
}

$quote_name = htmlentities(stripslashes($plain_name), ENT_QUOTES);
if ($quote_name == "") {
  $quote_name = "Anonymous";
}

// Figure out the taxonomy id for this node.
reset($node->taxonomy); // reset array pointer
$tid = key($node->taxonomy);
$parent_node = $node->field_parent_node[0]["value"];

$details = rpforum_get_post_details($node->nid);
if ($parent_node != -1) {
  $parent_details = rpforum_get_post_details($parent_node);
} else {
  $parent_details = $details;
}

// Is this topic (parent_node) locked?
$is_locked = ($parent_details["locked"] == "1") ? TRUE : FALSE;
// If we are an administrator, then it is not locked for us.
if (user_access("administer rpforums")) {
  if ($is_locked) {
    $admin_unlock = TRUE;
  }
  $is_locked = FALSE;
  
}


if (!$teaser) {
$pC .= "<a name='$node->nid'></a>";

  // If this is a topic, let's create the topic control links.
  if ($parent_node == -1) {
    $remove_topic_link = l(t("Remove topic"), "node/$node->nid/delete", array("query"=>"nid=$parent_node&tid=$tid&type=topic", "attributes" => array("class" => "rpforum-link-remove-topic")));
  }

$author_user = user_load($node->uid);
$author_profile = rpforum_build_user_profile($node->uid);

$pC .= "
        <div class='rpforum-post rpforum-post-$zebra'>
        
          <div class='rpforum-author-info'>";
if (function_exists("author_pane_theme") && variable_get("rpforum_use_author_pane", "") == "1") {
  // Meaning, this user has author pane installed.
  
  $pC .= theme("author_pane", $author_user);
}
else {
  // No author pane, so draw up your own stuff.
  $author_details = rpforum_get_author_details($node->uid);
  $author_posts = $author_details["posts"];
  $author_type = $author_details["author_type"];
  if ($node->uid > 0) {
    $author_avatar = rpforum_render_user_avatar($author_profile);
    $author_links = rpforum_render_author_links($author_profile);
  }
  
  $starclass = "";
  if (user_access("administer rpforums", $author_user)) {
    $starclass = "-admin";
  }
  
  $pC .= "  <div class='post-author'>$name</div>
            <div class='post-author-type'>$author_type</div>
            <div class='post-author-stars'>";
  if (is_numeric($author_details["tier"])) {
    for ($t = 1; $t <= $author_details["tier"]; $t++) {
      $pC .= "<div class='post-author-star$starclass'>&nbsp;</div>";
    }
  }
  $pC .= "</div>";
  
  $pC .= "<div class='post-author-posts'>" . t("Posts") . ": $author_posts</div>
            ";
  $pC .= "<div class='post-author-avatar'>$author_avatar</div>";
  $pC .= "<div class='post-author-links'>$author_links</div>";
  
}        

$topic_nid = ($parent_node == "-1") ? $node->nid : $parent_node;

$menu_base = variable_get("rpforum_menu_base", "rpforum");
$topic_url = "$menu_base/$tid/$topic_nid";
$img_path = $GLOBALS["base_url"] . "/" . drupal_get_path("module", "rpforum") . "/style/images/";
$message_icon = $node->field_message_icon[0]["value"] . ".gif";

//$self_link = l("<img src='$message_icon_path/$message_icon' title='perma-link to this post' class='rpforum-message-icon-image'>", $topic_url, array("query" => "goto=$node->nid", "html"=>true)) . " ";
// Since this is a "perma link" we need to use the find= structure, as
// its tid may change in the future.
$self_link = l("<img src='$message_icon_path/$message_icon' title='" . t("perma-link to this post") . "' class='rpforum-message-icon-image'>", $menu_base, array("query" => "find=$node->nid", "html"=>true)) . " ";

$pC .= "</div>"; //div rpforum-author-info
$pC .= "<div class='rpforum-between-author-info-and-post-content'></div>
          <div class='rpforum-post-content'>
            <div class='post-title'>$self_link$title</div>
            <div class='post-date'>$date</div>";
$pC .= "    <div class='post-body'>$content</div>";

// Was this node updated (but not simply set to sticky)?
if ($details["op"] == "update" && $details["only_modified_sticky"] != TRUE) {
  $update_user = user_load($node->revision_uid);
  $pC .= "<div class='post-updated'>" . t("Last updated on ") . rpforum_format_date($node->changed);
  if ($node->revision_uid > 0) {
    $by = t("by");
    $pC .= " $by " . theme("username", $update_user) . ".";
  }
  $pC .= "</div>";
}

// Is there a signature for this author?
if ($author_profile["signature"] != "") {
  
  $author_profile["signature"] = nl2br(trim($author_profile["signature"]));
  $author_profile["signature"] = rpforum_convert_bbcode_to_html($author_profile["signature"]);
  
  $pC .= "<div class='post-signature'>{$author_profile["signature"]}</div>";
}

// The control links (edit, delete, etc)...
if (strtolower($_POST["op"]) != "preview" && !$is_locked) {  // dont show on a preview post
  $pC .= "<div class='post-control-links'>";
  if ($admin_unlock) {
    $pC .= t("Because you are an administrator, you may still perform these functions,
            though this topic is locked to all other users:") . "<br>";
  }
  
  if (user_access("allow report rpforum abuse") && variable_get("rpforum_enable_report_abuse", 0) == 1) {
    $pC .= l(t("Report abuse"), "$menu_base/report-abuse/$tid/$topic_nid/$node->nid", array("attributes" => array("class" => "rpforum-link-report-abuse")));
    $pC .= " <span class='rp-forum-hyphen rp-forum-hypen-after-report-abuse'>-</span> ";
  }
  
  $pC .= l(t("Quote"), "node/add/rp-forum-post", array("query"=>"nid=$topic_nid&tid=$tid&token=" . rpforum_get_token($topic_nid . $tid) . "&quote=$node->nid", "attributes" => array("class" => "rpforum-link-quote")));
  if ($node->uid == $user->uid || user_access("edit any rp_forum_post content") || user_access("delete any rp_forum_post content") || rpforum_access_moderate_forum($tid)) {
    if (user_access("edit own rp_forum_post content") || user_access("edit any rp_forum_post content") || rpforum_access_moderate_forum($tid)) {
      $pC .= " <span class='rp-forum-hyphen rp-forum-hypen-after-quote'>-</span> ";
      $pC .= l(t("Edit"), "node/$node->nid/edit", array("query"=>"nid=$parent_node&tid=$tid&token=" . rpforum_get_token($parent_node . $tid), "attributes" => array("class" => "rpforum-link-edit")));
    if (user_access("delete own rp_forum_post content") || user_access("delete any rp_forum_post content") || rpforum_access_moderate_forum($tid)) {
      if ($parent_node != "-1") {
          $pC .= " <span class='rp-forum-hyphen rp-forum-hypen-after-edit'>-</span> ";
          $pC .= l(t("Delete"), "node/$node->nid/delete", array("query"=>"nid=$parent_node&tid=$tid&type=post", "attributes" => array("class" => "rpforum-link-delete")));
        }
        else {
          // We want to remove the entire topic!
          $pC .= " <span class='rp-forum-hyphen rp-forum-hypen-after-edit'>-</span> ";
          $pC .= $remove_topic_link;     
        }
      }
    }
    
    if (rpforum_access_moderate_forum($tid) && $parent_node != "-1") {
      $pC .= "<fieldset class='fs-rpforum-admin-post-control-links collapsible collapsed'>
                <legend>" . t("Admin options") . "</legend>";
      $pC .= "<div class='fieldset-wrapper admin-post-control-links'>";
      $pC .= l(t("Move post"), "$menu_base/move-post/$node->nid", array("attributes" => array("class" => "rpforum-link-move-post")));
      $pC .= " <span class='rp-forum-hyphen rp-forum-hypen-after-move-post'>-</span> ";      
      $pC .= l(t("Split topic"), "$menu_base/split-topic/$node->nid", array("attributes" => array("class" => "rpforum-link-split-topic")));
      $pC .= "</div>";
      $pC .= "</fieldset>";
    }
    
  }
  $pC .= "</div>";

  if (user_access("view rpforum submission details")) {
    $pC .= "<div class='post-ip-address'>IP: {$details["ip"]}</div>";
  }
  
}


$pC .= "<div style='clear: both;'>&nbsp;</div>";



$pC .= "  </div>"; // div rpforum-post-content


$pC .= "
        </div>"; // div rpforum-post
} 
else {
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  // TEASER!
  drupal_add_js(drupal_get_path("module", "rpforum") . "/js/rpforum.js");  
  
  $pC .= "<a name='$node->nid'></a>";
  
  if ($node->from_search == TRUE || $node->from_feed == TRUE) {
    $topic_nid = ($parent_node == "-1") ? $node->nid : $parent_node;
    $menu_base = variable_get("rpforum_menu_base", "rpforum");
    $topic_url = "$menu_base/$tid/$topic_nid";
    $img_path = $GLOBALS["base_url"] . "/" . drupal_get_path("module", "rpforum") . "/style/images/";
    $self_link = l("<img src='$img_path/last_post.gif' title='perma-link to this post'> $node->title", $topic_url, array("query" => "goto=$node->nid", "html"=>true));

    if ($node->from_feed == TRUE) {
      // No image!
      //$self_link = l("$node->title", $topic_url, array("query" => "goto=$node->nid", "html"=>true));
      $self_link = "";
      $author_user = user_load($node->uid);
      if ($author_user->uid > 0) {
        $name = $author_user->name;  // remove the link to the user's page.
      }
      $feed_body_style = "border: 1px solid #ccc; padding: 5px; margin: 5px;";
    }
    
    $pC .= "<div class='rpforum-search-result'>";
    $pC .= "<div class='search-title-link'>$self_link</div>";
  }
  
  $pC .= "
          <table class='rpforum-post-teaser rpformum-post-teaser-$zebra'>
          <tr class='post-teaser-header-row'>
            <td class='post-teaser-author'>" . t("Posted by") . " $name</td>
            ";
  if ($node->from_feed != true) {
    $pC .= "<td class='post-teaser-date'>" . t("Posted on") . " $date</td>";
  } 
  else { 
    $topic_node = node_load($topic_nid);
    $on = t("on");
    $pC .= "<td>$on $date, " . t("in topic") . " " . l("$topic_node->title", $GLOBALS["base_url"] . "/$topic_url") . "</td>"; 
  }
  $pC .= "
          </tr>
          <tr class='post-teaser-body-row'>
            <td colspan='2'>";
  if (arg(2) != "delete" && $node->from_search != TRUE && $node->from_feed != TRUE) {
    $quotevar = array(
      "rpforum_quotes" => array(
        "n$node->nid" => array("quote_body" => $quote_body, "quote_name" => $quote_name),
      )
    );
    drupal_add_js($quotevar, "setting");
    $pC .= "<div class='post-teaser-quote-link'>";
    $pC .= "<a href='javascript: rpforumInsertQuote($node->nid);' class='rpforum-link-insert-quote'>" . t("Insert quote") . "</a>";
    $pC .= "</div>";
  }
  $pC .= "              
              <div id='post-teaser-body-$node->nid' 
                   class='post-teaser-body'
                   style='$feed_body_style'>$content</div>
            </td>
          </tr>
          </table>";
  
  if ($node->from_search == true || $node->from_feed == TRUE) {
    $pC .= "</div>";
  }
  
  
  if ($node->from_blogger == true) {
    // If we are coming from blogger (requires the rpf_blogger module), then we
    // just want a very simple output.
    $author_user = user_load($node->uid);
    if ($author_user->uid > 0) {
      $name = $author_user->name;  // remove the link to the user's page.
    }    
    $pC = "";
    $pC .= "<div style='margin-top:20px;'>
		        <div><b>$name</b> " . t("said") . "...</div>
		        <div style='margin: 5px 0 5px 10px; padding-left: 10px; border-left: 0px solid gray;'>$content</div>
		        <div style='margin-left: 10px; font-size: 8pt; text-align:right;'><i>$date</i></div>
		        <div><hr></div>
	         </div>";
        
  }
  
  
  
}

print $pC;

?>