<?php // $Id: page.tpl.php,v 1.17 2009/05/07 17:00:40 jmburnz Exp $
/**
 * @file
 *  page.tpl.php
 *
 * Theme implementation to display a single Drupal page.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language; ?>" xml:lang="<?php print $language->language; ?>">
<head>
  <meta http-equiv="Content-Style-Type" content="text/css" />
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <!--[if IE]>
    <link type="text/css" rel="stylesheet" media="all" href="<?php print $base_path . $directory; ?>/ie.css" >
  <![endif]-->
  <?php print $scripts; ?>
</head>
<?php
  $pixture_width = theme_get_setting('pixture_width');
  $pixture_width = pixture_validate_page_width($pixture_width);
?>
<body id="pixture-reloaded" class="<?php print $body_classes; ?> <?php print $logo ? 'with-logo' : 'no-logo' ; ?>">

  <div id="skip-to-content">
    <a href="#main-content"><?php print t('Skip to main content'); ?></a>
  </div>

    <div id="page" style="width: <?php print $pixture_width; ?>;">

      <div id="header">

        <?php if ($site_logo): ?>
          <div id="logo"><?php print $site_logo; ?></div>
        <?php endif; ?>

        <div id="head-elements">

          <?php if ($search_box): ?>
            <div id="search-box">
              <?php print $search_box; ?>
            </div> <!-- /#search-box -->
          <?php endif; ?>

          <div id="branding">
            <?php if ($site_name): ?>
              <?php if ($title): ?>
                <div id="site-name"><strong><?php print $site_name; ?></strong></div>
              <?php else: /* Use h1 when the content title is empty */ ?> 
                <h1 id="site-name"><?php print $site_name; ?></h1>
              <?php endif; ?>
            <?php endif; ?>
            
            <?php if ($site_slogan): ?>
              <div id="site-slogan"><em><?php print $site_slogan; ?></em></div>
            <?php endif; ?>
          </div> <!-- /#branding -->

        </div> <!-- /#head-elements -->

        <?php if ($primary_menu || $superfish): ?>
          <!-- Primary || Superfish -->
          <div id="<?php print $primary_menu ? 'primary' : 'superfish' ; ?>">
            <div id="<?php print $primary_menu ? 'primary' : 'superfish' ; ?>-inner">
              <?php if ($primary_menu): ?>
                <?php print $primary_menu; ?>
              <?php elseif ($superfish): ?> 
                <?php print $superfish; ?>
              <?php endif; ?>
            </div> <!-- /inner -->
          </div> <!-- /primary || superfish -->
        <?php endif; ?>

    </div> <!--/#header -->

    <?php if ($header): ?>
      <div id="header-blocks" class="region region-header">
        <?php print $header; ?>
      </div> <!-- /#header-blocks -->
    <?php endif; ?>

    <div id="main" class="clear-block <?php print $header ? 'with-header-blocks' : 'no-header-blocks' ; ?>">

      <div id="content"><div id="content-inner">

        <?php if ($mission): ?>
          <div id="mission"><?php print $mission; ?></div>
        <?php endif; ?>

        <?php if ($content_top): ?>
          <div id="content-top" class="region region-content_top">
            <?php print $content_top; ?>
          </div> <!-- /#content-top -->
        <?php endif; ?>

        <div id="content-header" class="clearfix">
         <a name="main-content" id="main-content"></a>
          <?php // if ( arg(0) == 'forum' || strpos( drupal_lookup_path( 'alias', $_GET['q'] ), 'forum' ) === 0 ) { print $breadcrumb; } ?>
          <?php if ( arg(0) == 'forum' || $node->type == 'forum' ) { print $breadcrumb; } ?>
          <?php if ($title): ?><h1 class="title"><?php print $title; ?></h1><?php endif; ?>
          <?php if ($tabs): ?><div class="tabs"><?php print $tabs; ?></div><?php endif; ?>
          <?php if ($messages): print $messages; endif; ?>
          <?php if ($help): print $help; endif; ?>
        </div> <!-- /#content-header -->

        <div id="content-area">
          <?php print $content; ?>
        </div>

        <?php if ($content_bottom): ?>
          <div id="content-bottom" class="region region-content_bottom">
            <?php print $content_bottom; ?>
          </div> <!-- /#content-bottom -->
        <?php endif; ?>

        <?php if ($feed_icons): ?>
          <div class="feed-icons"><?php print $feed_icons; ?></div>
        <?php endif; ?>

      </div></div> <!-- /#content-inner, /#content -->

      <?php if ($left): ?>
        <div id="sidebar-left" class="region region-left">
          <?php print $left; ?>
        </div> <!-- /#sidebar-left -->
      <?php endif; ?>

	  
	  
      <?php if ($right): ?>
        <div id="sidebar-right" class="region region-right">
          &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="http://twitter.com/NACOtradeunion" target="blank"><img src="/sites/default/themes/pixture_reloaded/images/twitter.png" /></a>&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="http://www.facebook.com/pages/NACO-National-Association-of-Co-operative-Officials/116671255071290" target="blank"><img  src="/sites/default/themes/pixture_reloaded/images/facebook.png" /></a>
		  <br /><br />
		 <!-- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="/join"><img src="/sites/default/themes/pixture_reloaded/images/joinus.jpg" /></a>
		  <br /><br />-->
          <?php print $right; ?>
        </div> <!-- /#sidebar-right -->
      <?php endif; ?>

    </div> <!-- #main -->

    <div id="footer" class="region region-footer">
      <?php if ($footer): print $footer; endif; ?>
        <div id="footer-message">
          <?php print $footer_message; ?>
        </div> <!-- /#footer-message -->
    </div> <!-- /#footer -->

  </div> <!--/#page -->

  <?php if ($closure_region): ?>
    <div id="closure-blocks" class="region region-closure">
      <?php print $closure_region; ?>
    </div>
  <?php endif; ?>

  <?php print $closure; ?>

</body>
</html>
