<?php
// $Id: insert.tpl.php,v 1.1.4.3 2009/10/24 17:04:42 starnox Exp $

/**
 * @file
 * Template for Image Browser's insert window.
 *
 * Variables:
 * - $form
 * - $image
 * - $image_width
 * - $image_height
 * - $image_mime_type
 * - $image_size
 * - $image_owner
 * - $delete
 */
?>
<div class="header"><h2><?php print t('Insert'); ?></h2></div>
<div class="options">
  <?php print $form; ?>
</div>
<div class="details">
  <?php print $image; ?>
  <table>
    <tbody>
      <tr>
        <th><?php print t('Dimensions'); ?></th>
        <td><?php print $image_width; ?> x <?php print $image_height; ?> px</td>
      </tr>
      <tr>
        <th><?php print t('Mime type'); ?></th>
        <td><?php print $image_mime_type; ?></td>
      </tr>
      <tr>
        <th><?php print t('File size'); ?></th>
        <td><?php print $image_size; ?></td>
      </tr>
      <tr>
        <th><?php print t('Owner'); ?></th>
        <td><?php print $image_owner; ?></td>
      </tr>
    </tbody>
  </table>
</div>
<div class="footer">
  <a href="#" class="button insert"><?php print t('Insert'); ?></a>
  <a href="#" class="button close"><?php print t('Cancel'); ?></a>
  <?php print $delete; ?>
</div>