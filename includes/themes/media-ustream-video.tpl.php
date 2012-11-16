<?php

/**
 * @file media_ustream/includes/themes/media-ustream-video.tpl.php
 *
 * Template file for theme('media_ustream_video').
 *
 * Variables available:
 *  $uri - The media uri for the UStream video (e.g., ustream://recorded/123456).
 *  $video_id - The unique identifier of the UStream video/channel (e.g., 123456).
 *  $id - The file entity ID (fid).
 *  $url - The full url including query options for the UStream iframe.
 *  $options - An array containing the Media: UStream formatter options.
 *  $width - The width value set in Media: UStream file display options.
 *  $height - The height value set in Media: UStream file display options.
 *  $title - The title of the UStream video.
 *
 */

?>
<div class="<?php print $classes; ?> media-ustream-<?php print $video_id; ?>">
  <iframe
    class="media-ustream-player"
    width="<?php print $width; ?>"
    height="<?php print $height; ?>"
    title="<?php print $title; ?>"
    src="<?php print $url; ?>"
    style="border: 0">
  </iframe>
</div>
