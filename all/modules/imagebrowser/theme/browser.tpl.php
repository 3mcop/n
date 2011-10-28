<?php
// $Id: browser.tpl.php,v 1.1.4.5 2010/02/10 21:42:28 blixxxa Exp $

/**
 * @file
 * Template for Image Browser's pop-up window.
 *
 * Variables:
 * - $css
 * - $messages
 * - $upload
 * - $browser
 * - $javascript
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
    <div id="insert"></div>
    <div id="messages">
      <?php print $messages; ?>
    </div>
    <div class="header">
      <h2><?php print t('Browse'); ?></h2>
      <?php if ($upload) : ?>
      <div class="faux-button">
        <p class="loaded"><?php print t('Upload New'); ?></p>
        <p class="loading"><?php print t('Uploading...'); ?></p>
        <?php print $upload; ?>
      </div>
      <?php endif; ?>
    </div>
    <div id="browse">
      <div class="browse">
        <?php print $browser; ?>
      </div>
    </div>
    <?php print $javascript; ?>
  </body>
</html>