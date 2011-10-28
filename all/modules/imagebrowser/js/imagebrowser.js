// $Id: imagebrowser.js,v 1.1.4.4 2010/05/07 05:40:11 blixxxa Exp $

/**
 * @file imagebrowser.js
 *
 * All the javascript to make Image Browser work
 */

/**
 * Call correct API for insert
 */
function ib_insert(fid, preset, link, link_target, styles)
{
  $.getJSON(top.window.Drupal.settings.imagebrowser.basepath + 'index.php?q=imagebrowser/ajax&tag=[ibimage==' + fid + '==' + preset + '==' + link + '==' + link_target + '==' + styles + ']',
  function (data, textStatus) {
    ib_process_insert(data);
  });
}

/**
 * When they pick an image upload the file right away
 */
function ib_upload() {
  $(".faux-button").addClass('uploading');
  $(".faux-button > .loaded").hide();
  $(".faux-button > .loading").show();
  $("#imagebrowser-upload").trigger('submit');
  $("#imagebrowser-upload").hide();
}

/**
 * Make things happen on page load
 */
Drupal.behaviors.imagebrowser = function(context) {
  // Add on click functions for images
  $("#browse .browse .view-content ul li a").click(function() {
    $("#insert").fadeIn();
    $("#insert").html('<div class="header"><h2>'+Drupal.t('Insert')+'</h2></div><p class="loading-msg">'+Drupal.t('Loading...')+'</p><div class="footer"></div>');
    var href = $(this).attr("href");
    $("#insert").load(href, function() {
      // Close button
      $(".close").click(function() {
        $("#fade").fadeOut('slow');
        $("#insert").fadeOut('slow');
        return false;
      });
      
      // Insert button
      $(".insert").click(function() {
        var fid = $("#fid").val();
        var preset = $("#image_preset").val();
        
        if($("#link").val() == 'custom_url') {
          var link = $("#custom_url").val();
        }
        else {
          var link = $("#link").val();
        }
        
        var link_target = $("#link_target").val();
        
        var styles = $("#styles").val();
        
        ib_insert(fid, preset, link, link_target, styles);
        return false;
      });
      
      // Delete button
      $(".delete").click(function() {
        $(this).hide();
        $("#delete-confirm").show();
        return false;
      });
      
      // Cancel delete button
      $("#delete-confirm > .cancel").click(function() {
        $("#delete-confirm").hide();
        $(".delete").show();
        return false;
      });
      
      // Link selection
      $("#link").change(function() {
        if($("#link").val() == 'custom_url') {
          $("#custom_url-wrapper").show();
        }
        else {
          $("#custom_url-wrapper").hide();
        }
      });
      
      // Link target selection
      $("#link").change(function() {
        if($("#link").val() == 'none') {
          $("#link_target-wrapper").hide();
        }
        else {
          $("#link_target-wrapper").show();
        }
      });
    });
    return false;
  }).length == $("#browse .browse img").length || alert(Drupal.t("ImageBrowser might be broken because your custom views templates has altered the markup structure."));

  
  //Display messages if available
  if(!$("#messages:not(:has(*))").length) {
    $("#messages").fadeIn();
    setTimeout(function() {
      $("#messages").fadeOut('slow', function() {
        $("#messages").html('');
      });
    }, 2000);
  }
}