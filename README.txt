
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Configuration
 * Known issues
 * Upgrade notes
 * Further reading
 * Notes


INTRODUCTION
------------

The Media UStream 7.x-1.0 module intergrates with the Media module to offer an
easy way of adding UStream videos and channels to your website. The videos /
channels will be added as a 'file entity', this enables a wide varity of
options for developers. Other people have the best user experience possible
on the net to add, manage and maintain a large database of UStream videos /
channels on their site.

The module is complete, but currently lacks any functionality
beyond storing videos and live channels as Media fields.

What works:
- Adding recorded media and live channels from UStream to media 2.x
  (selector) enabled file fields.
- Thumbnails.
- Editing media via the 'manage files' page.


INSTALLATION
------------

Installation is like any other module, just place the files in the
sites/all/modules directory and enable the module on the modules page.


CONFIGURATION
-------------

- Add a new "file" type field to your content type or entity. Choose the widget
  type "Media file selector". You can also select an existing file field.

- While setting up the field (or after selecting "edit" on an existing field)
  enable:
    - Enabled browser plugins: "Web"
    - Allowed remote media types: "Video"
    - Allowed URI schemes: "youtube:// (YouTube videos)"

- Save the field and add a new post. Click on the 'Add media' button to add a file.

- An API Key is no longer needed.


KNOWN ISSUES
------------

- Coding style is ok, but the coding comments need work.
  MediaInternetUStreamHandler.inc & MediaUStreamStreamWrapper.inc
- Settings for Wysiwyg editor button are not configurable.
- Unable to upload video to UStream: True, see for more information:
  http://community.ustream.tv/ustream/topics/can_i_upload_and_record_to_ustream_without_broadcasting_live
  http://community.ustream.tv/ustream/topics/how_and_where_do_i_upload_video_to_my_ustream_basic_account


UPGRADE NOTES
-------------

We do not offer an upgrade path at this stage, but patches are always welcome.


BRAINSTORM
----------

- Add an option to be able to always render the channel logo.
- Cron function to update channel logos.
- Search Ustream > Search Media module is in development for this.
- Show live channels tab on the media browser popup.
- Login to UStream via your website and have the ability
  to add files from the UStream files you have access to.
- Keep on tracking the UStream developers for interesting developments.
- Create something like this for the 'media' module:
  http://drupalcode.org/project/media_gallery.git/blob/HEAD:/images/empty_gallery.png


FURTHER READING
---------------

- Media 2.x Overview, including file entities and view modes:
  http://drupal.stackexchange.com/questions/40229/how-to-set-media-styles-in-media-7-1-2-media-7-2-x/40685#40685
- Media 2.x Walkthrough: http://drupal.org/node/1699054
- A site with some useful information: http://drupalmedia.freeworldmedia.nl/book/howtos-guides


NOTES
-----

To set channels to autoplay, but have recordings not autoplay:
Install the 'entity view mode' module and create a new view mode for 'file'.
Then visit the 'admin/structure/file-types/manage/video/file-display', click
your new view mode and enable 'autoplay'. Then on your node type / view
select that new style to render 'channels' and the basic style for
'recordings' and your set. Yes, this could still use some love.
