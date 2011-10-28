
var rpforumAlreadyQuotedOnLoad;

Drupal.behaviors.rpforumInsertQuote = function() {
  
  try {
    if (Drupal.settings.rpforumQuote != "") {
      if (rpforumAlreadyQuotedOnLoad != true) {
        rpforumInsertQuote(Drupal.settings.rpforumQuote);
      }
      rpforumAlreadyQuotedOnLoad = true;
    }
   } catch (Exception) {
  }
  
}

function rpforumInsertQuote(nid) {
  // Insert the body of this nid between [quote][/quote] bbcode tags
  // into this page's main body textarea.
  //var quoteit = $("#post-teaser-body-" + nid).html();

  try {
  
    eval("var quoteit = Drupal.settings.rpforum_quotes.n" + nid + ".quote_body;");
    eval("var quote_name = Drupal.settings.rpforum_quotes.n" + nid + ".quote_name;");
        
    var msg = "[quote=\"" + quote_name + "\"]" + quoteit + "[/quote]";
  
    rpforumInsertIntoBody(msg, "");   
    
  } catch (Exception) {
  
  }  
  
    
  
}

function rpforumInsertIntoBody(text1, text2) {

  var elemento_dom=document.getElementById("edit-field-body2-0-value");
   if(document.selection){
    // This is our IE method.
    elemento_dom.focus();
    sel=document.selection.createRange();
    //alert(sel);
    sel.text = text1 + sel.text + text2;
    
    elemento_dom.focus();
    
    return;
   }
   if(elemento_dom.selectionStart||elemento_dom.selectionStart=="0"){
    var t_start=elemento_dom.selectionStart;
    var t_end=elemento_dom.selectionEnd;
    //alert(t_start + " " + t_end);
    var val_start=elemento_dom.value.substring(0,t_start);
    var val_end=elemento_dom.value.substring(t_end,elemento_dom.value.length);
    
    // If t_start and t_end, get what was in the middle!
    var middle_text = "";
    if (t_start != t_end) {
      //alert("here");
      middle_text = elemento_dom.value.substring(t_start, t_end);      
    }
    
    elemento_dom.value= val_start + text1 + middle_text + text2 + val_end;
   }else{
    elemento_dom.value += text1 + text2;
   }
  
   elemento_dom.focus();
   
}