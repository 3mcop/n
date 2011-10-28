// $Id: fckplugin.js,v 1.1.2.2 2009/10/24 17:04:41 starnox Exp $

// Get Drupal basePath
var FCKBasePath = top.window.Drupal.settings.basePath;
var FCKFilePath = top.window.Drupal.settings.imagebrowser.filepath;

// Create our own command to override FCK style
var InsertIBCommand = function(){ };

// We dont want the button to be toggled
InsertIBCommand.GetState=function() {
  return FCK_TRISTATE_OFF; 
}

// Open a popup window when the button is clicked
InsertIBCommand.Execute=function() {
  top.window.ib_fckeditor_dialog_open(FCK);
}

// Register the command
FCKCommands.RegisterCommand('ImageBrowser', InsertIBCommand );

// Register button
var oInsertIB = new FCKToolbarButton('ImageBrowser', 'ImageBrowser');
oInsertIB.IconPath = FCKConfig.PluginsPath + 'imagebrowser/ib_icon.gif';
FCKToolbarItems.RegisterItem( 'ImageBrowser', oInsertIB );

// Add buttons automatically
addToolbarElement('ImageBrowser', 'Basic', 0);
addToolbarElement('ImageBrowser', 'DrupalBasic', 0);
addToolbarElement('ImageBrowser', 'Default', 4);
addToolbarElement('ImageBrowser', 'DrupalFiltered', 0);
addToolbarElement('ImageBrowser', 'DrupalFull', 0);

FCKConfig.EditorAreaCSS = FCKConfig.EditorAreaCSS + ',' + FCKBasePath + FCKFilePath + '/ibstyles.css';

// Register Context Menu
FCK.ContextMenu.RegisterListener( {
  AddItems : function( menu, tag, tagName ) {
    if ( (tagName == 'img' || tagName == 'IMG') && tag.getAttribute('_fck_ib') ) {
      menu.RemoveAllItems();
      menu.AddItem( 'ImageBrowser', 'ImageBrowser', oInsertIB.IconPath ) ;
    }
  }
} );


// Add jQuery library
function appendScripts() {
  var jquery = document.createElement("script");
  jquery.type = "text/javascript";
  jquery.src = FCKBasePath + "misc/jquery.js";
  document.getElementsByTagName("head")[0].appendChild(jquery);
}
appendScripts();


//Save reference to the internal FCKeditor TagProcessor
//It must be called to process _fcksavedurl [#352704]
//FIXME: bad bad bad code, infinite loop when imgassist plugin is enabled
var FCKXHtml_TagProcessors_Img = FCKXHtml.TagProcessors['img'];

// We must process the IMG tags, change <img...> to [ibimage...]
FCKXHtml.TagProcessors['img'] = function( node, htmlNode )
{
  node = FCKXHtml_TagProcessors_Img( node, htmlNode ) ;

  if ( htmlNode.getAttribute('_fck_ib') ) {
    var IBString = FCKTools.ImageBrowserCode ( htmlNode );
    if( IBString )
      node = FCKXHtml.XML.createTextNode(IBString) ;
  }
  else{
    FCKXHtml._AppendChildNodes( node, htmlNode, false ) ;
  }
  return node ;
}

// Convert <img> tag into [ibimage]
FCKTools.ImageBrowserCode = function( IBObj )
{
  var IBString = [];
  if ( IBObj.getAttribute('_fck_ib') ) {
    IBString.push('[ibimage==');
    IBString.push(IBObj.getAttribute('_fck_ib'));
    IBString.push(']');
  }
  else {
    return false;
  }
  return IBString.join("");
}

// Find all occurences of [ibimage] and call ImageBrowserDecode
var FCKImageBrowserProcessor = FCKDocumentProcessor.AppendNew() ;
FCKImageBrowserProcessor.ProcessDocument = function( document )
{
  // get all elements in FCK document
  var elements = document.getElementsByTagName( '*' ) ;

  // check every element for childNodes
  var i = 0;
  while (element = elements[i++]) {
    var nodes = element.childNodes;

    var j = 0;
    while (node = nodes[j++]) {
      if (node.nodeName == '#text') {
        var index = 0;
        while (aPlaholders = node.nodeValue.match( /\[ibimage[^\[\]]+\]/g )) {
          index = node.nodeValue.indexOf(aPlaholders[0]);
          if (index != -1) {
            var oImgBr = FCKTools.ImageBrowserDecode ( aPlaholders[0] );

            var substr1 = FCK.EditorDocument.createTextNode( node.nodeValue.substring(0, index) );
            var substr2 = FCK.EditorDocument.createTextNode( node.nodeValue.substring(index + aPlaholders[0].length) );

            node.parentNode.insertBefore( substr1, node ) ;
            node.parentNode.insertBefore( oImgBr, node ) ;
            node.parentNode.insertBefore( substr2, node ) ;

            node.parentNode.removeChild( node ) ;
            if (node) node.nodeValue='';
          }
        }
      }
    }
  }
}

// [ibimage] -> <img>
FCKTools.ImageBrowserDecode = function( IBString )
{
  var IBTmp = FCK.EditorDocument.createElement( 'img' );
  var s = {};
  
  var IBarg = IBString.match( /\[ibimage\s*([^\]]*?)\]/ );
  var s = IBarg[1].split("==");

  //[ibimage=={FID}=={PRESET_NAME}=={LINK_PRESET_NAME/URL}=={STYLES}]
  $.getJSON(FCKBasePath + 'index.php?q=imagebrowser/ajax&tag=' + IBString,
  function (data, textStatus) {
    var oImg, oDiv = FCK.EditorDocument.createElement( 'div' ) ;
    oDiv.innerHTML = '<img src="' + data.image_url + '" class="ibimage ' + data.styles + '" />';
    for (var i=0; i<oDiv.childNodes.length; i++) {
      if (oDiv.childNodes[i].nodeType == 1) {
        switch (oDiv.childNodes[i].tagName.toLowerCase()) {
          case 'a':
          oImg = oDiv.childNodes[i].childNodes[0];
          case 'img':
          if (!oImg) {
            oImg = oDiv.childNodes[i];
          }
          $(IBTmp).attr("src", $(oImg).attr("src")).attr("class", 'ibimage ' + data.styles);
        }
      }
    }
  }
  );
  
  var _fck_ib = [];
  if (s[0]) {
    _fck_ib.push(s[0]);
  }
  if (s[1]) {
    _fck_ib.push(s[1]);
  }
  if (s[2]) {
    _fck_ib.push(s[2]);
  }
  if (s[3]) {
    _fck_ib.push(s[3]);
  }
  if (s[4]) {
    _fck_ib.push(s[4]);
  }
  if (s[5]) {
    _fck_ib.push(s[5]);
  }
  $(IBTmp).attr("_fck_ib", _fck_ib.join("=="));
  
  return IBTmp;
}