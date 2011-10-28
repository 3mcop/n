<?php

function rpforum_set_search_data($node) {
  // Updates the post_search_data table with this new information,
  // assisting in searching.
  $vid = variable_get("rpforum_nav_vocabulary", "");      
  
  $title = trim($node->title);
  $body = trim($node->field_body2[0]["value"]);
  
  $tid = $node->taxonomy[$vid];
   
  // Get the terms for this post.
  // Delete any old terms first.
  db_query("DELETE FROM {rpforum_post_search_terms}
            WHERE nid = %d", $node->nid);

  // Now, discover the terms.
  $temp = explode(" ", $body);
  foreach($temp as $t) {
    $t = trim(preg_replace("/[^a-zA-Z0-9]/", "", $t));
    
    if (strlen($t) < 4) continue;
    
    // Okay, we can insert $t now as a term.
    db_query("INSERT INTO {rpforum_post_search_terms}
              (nid, term) VALUES (%d, '%s')", $node->nid, $t);    
  }
  
  // Okay, now insert the rest of the data into our search_data table.
  db_query("UPDATE {rpforum_post_search_data}
            SET title = '%s',
            body = '%s',
            tid = %d,
            updated = %d
            WHERE nid = %d", $title, $body, $tid, time(), $node->nid);
  if (db_affected_rows() == 0) {
    db_query("INSERT INTO {rpforum_post_search_data}
              (title, body, tid, updated, nid)
              VALUES ('%s', '%s', %d, %d, %d)
              ", $title, $body, $tid, time(), $node->nid);
  }
  
}



function rpforum_display_search() {
  $rtn = "";
  
  drupal_add_css(drupal_get_path("module", "rpforum") . "/style/rpforum.css");
  
  $search_string = $_REQUEST["search_string"];
  $rtn .= drupal_get_form("rpforum_search_form", $search_string);
  
  // Now, we can actually perform the search and display results...
  $results_per_page = 10;
  
  // Thanks to our validate function, our results should be neatly ordered
  // in the search_results page.  Let's query that using a nifty
  // pager query.
  $is_empty = true;
  $query = "SELECT nid FROM {rpforum_user_search_results}
            WHERE ssid = '" . db_escape_string(session_id()) . "' 
            ORDER BY score DESC ";
  $res = pager_query($query, $results_per_page);
  while($cur = db_fetch_array($res)) {
    $node = node_load($cur["nid"]);
    
    // Make sure the user has access to view the forum that this node is in.

    $tid = key($node->taxonomy);
    if (!rpforum_access_forum($tid)) {
      continue;
    }
        
    $node->from_search = true;
    $content .= node_view($node, TRUE, FALSE, FALSE);
    $is_empty = false;
  }
  
  if ($is_empty == false) {
    
    // Display the pager at the top of the screen?
    if (variable_get("rpforum_display_pager_at_top", "1") == "1") {
      $top_pager = theme("pager", NULL, $results_per_page);
      if ($top_pager) {
        $rtn .= "<div class='rpforum-pager-top'>$top_pager</div>";
      }
    }    
    
    $rtn .= "<h2>Search results</h2><div class='rpforum-search-results'>$content</div>";
  }
  else if ($search_string != ""){
    $rtn .= "<h2>" . t("No results") . "</h2>
            " . t("Sorry, no results could be found from your search.") . "
            <br>" . t("Searches must contain at least 4 characters.");            
  }
  
  
  $rtn .= theme("pager", NULL, $results_per_page);
    
  
  
  
  
  return $rtn;
}


function rpforum_display_search_block() {
  
  drupal_add_css(drupal_get_path("module", "rpforum") . "/style/rpforum.css");
  
  $rtn = "";
  
  $rtn .= "<div id='rpforum-search-block'>";
  
  $rtn .= drupal_get_form("rpforum_search_form", "", true);
  
  $rtn .= "</div>";
  
  return $rtn;
}



function rpforum_search_form($form_info, $search_string, $bool_is_block = false) {
  // Displays the form for which we might search.
  
  $form = array();
  
  $form["mark" . $m++] = array(
    "#value" => "<div class='rpforum-search-input-form'>",
  );
  
  $form["search_string"] = array(
    "#type" => "textfield",
    "#default_value" => $search_string,
  );
  
  $form["submit"] = array(
    "#type" => "button",
    "#value" => t("Search"),
  );
  
  if ($bool_is_block) {
    // We want to make some modifications if this is the block.
    
    $form["search_string"]["#size"] = 15;
    $form["search_string"]["#title"] = variable_get("rpforum_search_block_title", "Search the forums");
    $form["from_block"] = array(
      "#type" => "hidden",
      "#value" => "yes",
    );
    
    
  }

  $form["mark" . $m++] = array(
    "#value" => "</div>",
  );
  
  
  
  return $form;


}

function rpforum_search_form_validate($form, $form_state) {
  
  $menu_base = variable_get("rpforum_menu_base", "rpforum");
  
  $search_string = $form_state["values"]["search_string"];
  
  
  // Begin by deleting any old search_results from the table for this user.
    db_query("DELETE FROM {rpforum_user_search_results}
              WHERE ssid = '%s' ", session_id());
  
  $temp = explode(" ", $search_string);
  $term_string = "";
  $terms = array();
  foreach($temp as $t) {
    $t = trim($t);
    if (strlen($t) < 4) continue;
    
    $term_string .= " \n !FIELDNAME LIKE '%%" . db_escape_string($t) . "%%' OR";
    $terms[] = trim(preg_replace("/[^a-zA-Z0-9]/", "", $t));
  }
  
  $term_string = substr($term_string, 0, -2);
  
  if ($term_string != "") {

    // The first thing we want to do is get our relavance array...
    $rel_array = array();
        
    // Begin by testing the titles!
    $ts = str_replace("!FIELDNAME", "a.title", $term_string);
    $query = "SELECT nid FROM {rpforum_post_search_data} a
              WHERE 1 AND
                  ($ts) ";
    $res = db_query($query);
    while($cur = db_fetch_array($res)) {
      $rel_array[$cur["nid"]]++;
    }
        
    // Now, test the bodies
    $ts = str_replace("!FIELDNAME", "a.body", $term_string);
    $query = "SELECT nid FROM {rpforum_post_search_data} a
              WHERE 1 AND
                  ($ts) ";
    $res = db_query($query);
    while($cur = db_fetch_array($res)) {
      $rel_array[$cur["nid"]]++;
    }
    
    
    // Now, search through the terms table itself for each term.
    foreach($terms as $t) {
      $res = db_query("SELECT nid FROM {rpforum_post_search_terms} 
              WHERE term LIKE '%%%s%%'", $t);
      while($cur = db_fetch_array($res)) {
        $rel_array[$cur["nid"]]++;
      }      
    }
    
    
    // Now, let's insert all of the rel_array into our search results
    // table for this user.
    foreach($rel_array as $nid=>$score) {
      db_query("INSERT INTO {rpforum_user_search_results}
                (ssid, nid, score, datetime) VALUES ('%s', %d, %d, NOW())
                ", session_id(), $nid, $score);
    }
    
    
  
  }
  
  if ($form_state["values"]["from_block"] == "yes") {
    drupal_goto("$menu_base/search", "search_string=$search_string");
  }
  
  
}





?>