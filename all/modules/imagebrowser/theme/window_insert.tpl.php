<?php
// $Id: window_insert.tpl.php,v 1.1.2.7 2009/07/19 14:30:51 starnox Exp $

/**
 * @file
 * Template for Image Browser's insert window.
 */
?>
<h2><?php print t('Insert'); ?></h2>
<div class="presets">
  <input type="hidden" id="ib_image_alt" value="<?php print $alt_text; ?>" />
  <h3><?php print t('Image Preset'); ?></h3>
  <div class="image box">
    <select id="ib_image_preset" name="ib_image_preset"><?php print $options; ?></select>
  </div>
  <h3><?php print t('Link'); ?> <span>(<?php print t('Optional'); ?>)</span></h3>
  <div class="link box">
      <select id="ib_link_options" name="ib_link_options"><?php print $link_options; ?></select>
      <input type="text" id="ib_link_custom" name="ib_link_custom" value="<?php print $customurl; ?>" />
      <select id="ib_link_target" name="ib_link_target"><?php print $target; ?></select>
  </div>
  <h3><?php print t('Alignment'); ?> <span>(<?php print t('Optional'); ?>)</span></h3>
  <div class="alignment box">
    <select id="ib_alignment" name="ib_alignment"><?php print $alignment; ?></select>
  </div>
  <div class="spacer"></div>
</div>
<div class="details">
  <img src="<?php print $image_url; ?>" alt="Thumbnail" />
  <table>
    <tbody>
      <tr>
        <th><?php print t('Title'); ?></th>
        <td><?php print $title; ?></td>
      </tr>
      <tr>
        <th><?php print t('Dimensions'); ?></th>
        <td><?php print $image_width; ?> x <?php print $image_height; ?> px</td>
      </tr>
      <tr>
        <th><?php print t('Mime type'); ?></th>
        <td><?php print $image_mime_type; ?></td>
      </tr>
      <tr>
        <th><?php print t('Owner'); ?></th>
        <td><?php print $username; ?></td>
      </tr>
    </tbody>
  </table>
  <div class="options">
    <?php print $edit_options; ?>
  </div>
  <div class="confirm">
    <p><?php print t('Are you sure you want to delete this image?'); ?></p>
    <a href="#" class="button delete_confirm"><?php print t('Yes, Delete'); ?></a>
    <a href="#" class="button cancel"><?php print t('No, Cancel'); ?></a></div>
</div>
<div class="footer">
  <a href="#" class="button insert"><?php print t('Insert'); ?></a>
  <a href="#" class="button close"><?php print t('Close'); ?></a>
</div>