<?php

/**
 * @file
 * The media_ustream/includes/themes/media_ustream.theme.inc file.
 *
 * Theme and preprocess functions for Media: UStream.
 */

/**
 * Preprocess function for theme('media_ustream_video').
 */
function media_ustream_preprocess_media_ustream_video(&$variables) {
  // Build the URI.
  $wrapper = file_stream_wrapper_get_instance_by_uri($variables['uri']);
  $parameters = $wrapper->get_parameters();
  $variables['video_id'] = check_plain($parameters['video_id']);

  // Make the file object available.
  $file_object = file_uri_to_object($variables['uri']);

  // Build the base query string.
  $query = array(
    // TODO: Keep track of this.
    // No idea what this is for, but ustream incudes
    // it in the embed tags which they generate.
    'v' => 3,
    // Enables hardware acceleration.
    'wmode' => 'direct',
    // TODO: And keep track of this.
    // The 'ub', 'lc', 'oc', and 'uc' GET args are hex color strings
    // which set the color of the UI controls.
    // TODO: Figure out how to program these for customization by the user.
    // coredumperror 2012/11/14: I have no idea what these 2-letter
    // combinations actually stand for,
    // and 'lc' and 'oc' don't seem to actually do anything.
  );

  // These GET args default to 0. If the option is true, set value to 1.
  foreach (array('autoplay') as $option) {
    if ($variables['options'][$option]) {
      $query[$option] = 1;
    }
  }

  if ($variables['options']['protocol_specify']) {
    $protocol = $variables['options']['protocol'];
  }
  else {
    $protocol = '';
  }

  // Add some options as their own template variables.
  foreach (array('width', 'height') as $themevar) {
    $variables[$themevar] = $variables['options'][$themevar];
  }

  // Do something useful with the overridden attributes from
  // the file object. We ignore alt and style for now.
  if (isset($variables['options']['attributes']['class'])) {
    if (is_array($variables['options']['attributes']['class'])) {
      $variables['classes'] = array_merge($variables['classes_array'], $variables['options']['attributes']['class']);
    }
    else {
      // Provide nominal support for Media 1.x.
      $variables['classes'][] = $variables['options']['attributes']['class'];
    }
  }

  // Add template variables for accessibility.
  $variables['title'] = check_plain($file_object->filename);

  // Build the iframe's src URL, with playback options set via query string.
  if ($parameters['recorded']) {
    $url = "$protocol//www.ustream.tv/embed/recorded/{$variables['video_id']}";
  }
  else {
    $url = "$protocol//www.ustream.tv/embed/{$variables['video_id']}";
  }

  // @TODO: Find an easy/ not too expensive way to get the video description
  // to use for the alternative content.
  $variables['alternative_content'] = t('Video of @title', array('@title' => $variables['title']));

  $variables['url'] = url($url, array('query' => $query, 'external' => TRUE));
}
