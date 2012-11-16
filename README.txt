A clone of the media_youtube module, but now for UStream. Video and channel functionality is working, but search capability is currently non-functional.

This module is complete, but currently lacks any functionality beyond storing videos and live channels as Media fields.

What works:
- Adding recorded media and live channels from UStream to media 2.x (selector) enabled file fields.
- Thumbnails.

What needs work:
- Media popup search + media popup ustream page.


Couple ideas:

Show all latest content from
http://api.ustream.tv/html/stream/all/getAllNew
on the UStream tab when adding media

Options to search on ustream for users/videos:
http://api.ustream.tv/json/user/recent/search/username:like:drupal
http://api.ustream.tv/json/video/recent/search/title:like:drupal

Wanna know more? See:
http://developer.ustream.tv/
http://developer.ustream.tv/data_api/docs


PS. API Key is no longer needed.

Where we are now:
- Currently investigating requirements and extra options we might need to add.
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
