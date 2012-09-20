<?php

/**
 * @file media_ustream/includes/themes/media-ustream-video.tpl.php
 *
 * Template file for theme('media_ustream_video').
 *
 * Variables available:
 *  $uri - The media uri for the UStream video (e.g., ustream://v/xsy7x8c9).
 *  $video_id - The unique identifier of the UStream video (e.g., xsy7x8c9).
 *  $id - The file entity ID (fid).
 *  $url - The full url including query options for the Youtube iframe.
 *  $options - An array containing the Media Youtube formatter options.
 *  $api_id_attribute - An id attribute if the Javascript API is enabled; 
 *  otherwise NULL.
 *  $width - The width value set in Media: Youtube file display options.
 *  $height - The height value set in Media: Youtube file display options.
 *
 */

?>
<div class="media-ustream-outer-wrapper media-ustream-<?php print $id; ?>" id="media-ustream-<?php print $id; ?>" style="width: <?php print $width; ?>px; height: <?php print $height; ?>px;">
  <div class="media-ustream-preview-wrapper" id="<?php print $wrapper_id; ?>" width="<?php print $width; ?>" height="<?php print $height; ?>">
    <?php print $output; ?>
  </div>
</div>
