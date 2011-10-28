<?php
// $Id: window.tpl.php,v 1.1.2.11 2009/07/19 14:30:51 starnox Exp $

/**
 * @file
 * Template for Image Browser's pop-up window.
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title><?php print t('Image Browser'); ?></title>
    <?php print $css; ?>
  </head>
  <body>
    <div id="fade"></div>
    <div id="insert"></div>
    <div id="edit"></div>
    <div id="messages">
      <h2><?php print t('Messages'); ?></h2>
      <ul class="log"></ul>
      <div class="footer"><a href="#" class="button close">Close</a></div>
    </div>
    <div id="upload">
      <?php print $upload; ?>
    </div>
    <div id="browse">
      <p class="help"><?php print t('Please select an image to insert, use filters to narrow down your results.'); ?></p>
      <div class="selectors">
        <a href="#" class="open-messages">Messages <span class="num">0</span></a>
        <a href="#" class="open-filters">Filters</a>
      </div>
      <div class="browse">
        <?php print $browser; ?>
      </div>
    </div>
    <?php print $javascript; ?>
  </body>
</html>