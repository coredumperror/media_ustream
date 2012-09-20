A clone of the media_youtube module, but now for UStream. (still needs loads of work)

This module is a work in progress, but it is stable enough for sites that want to test it.

What works:
- adding recorded media from UStream to media (selector) enabled fields.
- thumbnails.

Everything else still needs work.
- video names don't work
- html5 embed needs work (initial attempt is implemented already, the 'html5' option has no effect anymore)
- need to keep up with media_youtube/media_vimeo for some patches and port those patches if required and report back issues we find (like some pieces of text that can't be translated in media_youtube and this module)(and look for a possible fix for those obviously)

Couple ideas:

Show all latest content from
http://api.ustream.tv/html/stream/all/getAllNew
on the UStream tab when adding media

Options to search on ustream for users/videos:
http://api.ustream.tv/html/user/recent/search/username:like:drupal
http://api.ustream.tv/html/video/recent/search/title:like:drupal

(it's now all html links/output, change 'html' to json/php/xml for your favorite output-'flavour')

Wanna know more? See:
http://developer.ustream.tv/
http://developer.ustream.tv/data_api/docs


PS. API Key? Looks like we don't need it anymore.


All TODO's that are still in place are mostly coming straight from the media_youtube module, but large parts of the code have already been replaced and re-created in the way media_vimeo deals with things. The module is in a working state, but it still needs loads of work. (and if i continue to develop this, some things need to be taken care of first)(like how to pay the rent n such ;) )


Where we are now:
- Currently investigating requirements and extra options we might need to add.
- Checking out styles 2.x.
- Testing if we could benefit from the media_vimeo module's way of replacing the <object> tags with an iframe.
- Trying out some ways to implement a view page on the media browser popup for ustream (and what we could use it for)(latest videos/search users/search videos) and how to implement these in a proper way.


Some extra info:


http://developer.ustream.tv/
http://developer.ustream.tv/data_api/docs

http://api.ustream.tv/html/video/123456/getInfo

http://api.ustream.tv/html/stream/all/getAllNew

http://api.ustream.tv/html/user/recent/search/username:like:drupal
http://api.ustream.tv/html/video/recent/search/title:like:drupal


http://drupalcontrib.org/api/drupal/contributions!media!modules!media_internet!media_internet.module/7
http://api.drupalhelp.net/api/media/functions/7.2?page=1
http://api.drupalhelp.net/api/media_youtube/functions/7


new style
<iframe width="480" height="386" src="http://www.ustream.tv/embed/recorded/123456?wmode=direct" scrolling="no" frameborder="0" style="border: 0px none transparent;">    </iframe>
<br /><a href="http://www.ustream.tv/" style="padding: 2px 0px 4px; width: 400px; background: #ffffff; display: block; color: #000000; font-weight: normal; font-size: 10px; text-decoration: underline; text-align: center;" target="_blank">Video streaming by Ustream</a>

new style no hw accel (wmode not set)
<iframe width="480" height="386" src="http://www.ustream.tv/embed/recorded/123456" scrolling="no" frameborder="0" style="border: 0px none transparent;">    </iframe>
<br /><a href="http://www.ustream.tv/" style="padding: 2px 0px 4px; width: 400px; background: #ffffff; display: block; color: #000000; font-weight: normal; font-size: 10px; text-decoration: underline; text-align: center;" target="_blank">Video streaming by Ustream</a>

old style
<object width="480" height="386" classid="clsid:d27cdb6e-ae6d-11cf-96b8-444553540000">
  <param name="wmode" value="direct"/>
  <param name="flashvars" value="vid=123456&amp;autoplay=false"/>
  <param name="allowfullscreen" value="true"/>
  <param name="allowscriptaccess" value="always"/>
  <param name="src" value="http://www.ustream.tv/flash/viewer.swf"/>
  <embed flashvars="vid=123456&amp;autoplay=false" width="480" height="386" allowfullscreen="true" allowscriptaccess="always" src="http://www.ustream.tv/flash/viewer.swf" type="application/x-shockwave-flash"></embed>
</object>
<br /><a href="http://www.ustream.tv/" style="padding: 2px 0px 4px; width: 400px; background: #ffffff; display: block; color: #000000; font-weight: normal; font-size: 10px; text-decoration: underline; text-align: center;" target="_blank">Video streaming by Ustream</a>

old style no hw accel (wmode not set)
<object width="480" height="386" classid="clsid:d27cdb6e-ae6d-11cf-96b8-444553540000">
  <param name="flashvars" value="vid=123456&amp;autoplay=false"/>
  <param name="allowfullscreen" value="true"/>
  <param name="allowscriptaccess" value="always"/>
  <param name="src" value="http://www.ustream.tv/flash/viewer.swf"/>
  <embed flashvars="vid=123456&amp;autoplay=false" width="480" height="386" allowfullscreen="true" allowscriptaccess="always" src="http://www.ustream.tv/flash/viewer.swf" type="application/x-shockwave-flash"></embed>
</object>
<br /><a href="http://www.ustream.tv/" style="padding: 2px 0px 4px; width: 400px; background: #ffffff; display: block; color: #000000; font-weight: normal; font-size: 10px; text-decoration: underline; text-align: center;" target="_blank">Video streaming by Ustream</a>


old (youtube clone) style (also works obviously, but no flash fallback if html5 is not supported, current way offers this posibility)
  <div class="content">
    <div class="media-ustream-video media-ustream-1">
  <iframe class="media-ustream-player" width="640" height="400" src="https://www.ustream.tv/embed/recorded/123456?wmode=opaque" frameborder="0" allowfullscreen></iframe>
</div>

