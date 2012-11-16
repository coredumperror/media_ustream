﻿<?php

/**
 * @file media_ustream/includes/themes/media-ustream-video-legacy.tpl.php
 *
 * Legacy markup template file for theme('media_ustream_video').
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
 * IMPORTANT: This file is provided to ease the upgrade path from Media:
 *  UStream 7.x-1.x and 7.x-2.0-alpha2 versions by maintaining the legacy
 *  7.x-1.x markup. The markup has been simplified and improved in the 2.0
 *  and 2.x-dev versions of media-ustream-video.tpl.php. It is recommended
 *  that you revise any css or javascript that requires the old markup and
 *  then delete this file.
 */
 
?>
<div class="media-ustream-outer-wrapper" id="media-ustream-<?php print $id; ?>" width="<?php print $width; ?>" height="<?php print $height; ?>">
  <div class="media-ustream-preview-wrapper" id="<?php print $video_id . "_" . $id; ?>">
    <iframe class="media-ustream-player" <?php print $api_id_attribute; ?>
      width="<?php print $width; ?>"
      height="<?php print $height; ?>"
      src="<?php print $url; ?>"
      style="border: 0">
    </iframe>
  </div>
</div>
