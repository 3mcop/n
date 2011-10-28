=======================
RP Forum version 1.6.3.1b
=======================
Richard Peacock (richard@richardpeacock.com)

This module is meant to be a simple, all-in-one forum solution for Drupal 6.
It includes a convertor to help you move from an SMF (simple machines forum)*.

It provides many of the same features as other popular open-source forums,
like phpbb and smf.  While it does not have as many features as Drupal's
premiere forum solution, Advanced Forum, it requires considerably fewer
supporting modules and configurations (Advanced Forum requires between 15 - 20
additional modules, all with separate configurations, to replicate the 
functionality of phpbb or SMF).  From experience, I can tell you that setting up
and themeing Advanced Forum with all those extra modules can take hours.

RP Forum requires just 3 additional modules to run, and comes with the 
most common features expected of a forum.  That said, if
you feel like you would prefer the flexibility of Advanced Forum, please
give it a try; it is an excellent solution.  RP Forum is designed for
novice (or lazy ;) webmasters and their less-than-tech-savvy clients who
just want a basic forum solution up and running in 15 minutes.

Requirements:  Views, CCK, Install Profile API (http://drupal.org/project/install_profile_api)
Recommended: captcha (http://drupal.org/project/captcha)
             Privatemsg (http://drupal.org/project/privatemsg)

      
===============================================
Features (included without additional modules!)
===============================================

+ Ability to ban users based on IP and IP ranges
+ Filter out curse words (or any unwanted text) from posts.
+ User signatures and avatars
+ Subforums (1 level deep under regular forums)
+ Automatically word-wraps long body URLs and strings
+ User ranks based on # of posts
+ Users may move topics, split topics, and move posts
+ Custom search functionality
+ Includes an attractive default theme
+ Simple WYSIWYG BBCode editor - may be disabled, and fckeditor or
  any other editor may be used.
+ Smiley package (included with WYSIWYG editor).
+ Automatic RSS/Atom feed of posts (may be disabled).
+ Users can "subscribe" (be emailed of replies) to posts.
+ Users can report abusive posts    
+ Moderators on a per-forum basis.
+ Access (view & edit) based on user's role, on a per-forum basis.
+ Automatically interfaes with Privatemsg and User Panel.
+ All posts are CCK nodes, not comments.  So users can easily
  attach images or files or any other field type simply by 
  adding the field to the rp_forum_post content type.


*This module also comes with a convertor to help you convert from an SMF forum, but be warned
that it is rough and needs to be hand-configured for your setup.  It will
theoretically copy over your posts and users from an old SMF forum's database, and allow
your users to log in to RP Forum using their SMF username and password.
  
  
===========================================
Installation and Configuration Instructions
===========================================

After installing and enabling the required modules, install rpforum
as you would any other module.

Visit admin/settings/rpforum to configure.  By default, the menu root
for rpforum will be at example.com/rpforum.  You may change that here.

Next, go to the "Forums" tab.  You must create at least one Container
and one Forum.  Forums go inside of Containers.  To do this, drag the Forum
row UNDER and TO THE RIGHT of the Container.  Containers may have multiple
forums.

Finally, before your forum will be useable by the general public, you must enable
certain permissions.  Give your visitors/users the node->Create rp_forum_post content
permission, as well as the Edit or Delete permissions as you see fit.  Also, give
visitors/users the rpforum->access rpforums permission.

If you are allowing anonymous users to post on your forum, it is HIGHLY
recommended you install the captcha module (http://drupal.org/project/captcha)
to cut down on spam submissions.  When configuring that module, use
the form ID of "rp_forum_post_node_form" to place a CAPTCHA on the post forms.

========================================
Fine Print

This code is considered "open source," and is released under the 
GNU Public License version 3 or any later version (http://www.gnu.org/copyleft/gpl.html).  
In a nut shell, you can change this code however you want, and redistribute it, 
but ONLY if it is ALSO under the GNU Public License version 3 or later.  

If you do use this software, I would appreciate credit and a link back to this 
site (richardpeacock.com), though this is not required.