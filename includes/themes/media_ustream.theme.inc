<?php

/**
 * @file media_ustream/includes/themes/media_ustream.theme.inc
 *
 * Theme and preprocess functions for Media: UStream.
 */

/**
 * Preprocess function for theme('media_ustream_video').
 */
function media_ustream_preprocess_media_ustream_video(&$variables) {

  // Build the URI.
  $wrapper = file_stream_wrapper_get_instance_by_uri($variables['uri']);
  $parts = $wrapper->get_parameters();
  $variables['video_id'] = check_plain($parts['v']);

  // Parse options and build the query string. Only add the option to the query
  // array if the option value is not default. Be careful, depending on the
  // wording in media_ustream.formatters.inc, TRUE may be query=0.
  // @see 
  $query = array();

  // Make css z-index work with flash object. Must be the first parameter.
  $query['wmode'] = 'opaque';

  // These queries default to 0. If the option is true, set value to 1.
  foreach (array('autoplay') as $option) {
//  foreach (array('autoplay', 'enablejsapi', 'loop', 'modestbranding') as $option) {
    if ($variables['options'][$option]) {
      $query[$option] = 1;
    }
  }

  $query['playerapiid'] = drupal_html_id('media-ustream-' . $variables['video_id']);
  $variables['api_id_attribute'] = 'id="' . $query['playerapiid'] . '" ';

/**
  if ($variables['options']['enablejsapi']) {
    // Add a query ID and identical html ID if js API is set.
    $query['playerapiid'] = drupal_html_id('media-ustream-' . $variables['video_id']);
    $variables['api_id_attribute'] = 'id="' . $query['playerapiid'] . '" ';

    //Add the origin for improved security
    $variables['options']['origin'] ? $query['origin'] = $variables['options']['origin'] : '';
  }
  else {
    $variables['api_id_attribute'] = NULL;
  }
**/

  // Currently, loop only works with a playlist. Make fake playlist out of a
  // single video.
  // @see 
/**
  if ($variables['options']['loop']) {
    $query['playlist'] = $variables['video_id'];
  }
**/

  // These queries default to 1. If the option is false, set value to 0.
/**
  foreach (array('rel', 'showinfo') as $option) {
    if (!$variables['options'][$option]) {
      $query[$option] = 0;
    }
  }
**/

  // These queries default to 1. Option wording is reversed, so if the option
  // is true, set value to 0.
/**
  if ($variables['options']['controls']) {
    $query['controls'] = 0;
  }
**/
  // Reverse the reversed option so that it makes sense in the tpl.
/**
  if ($variables['options']['controls'] = 1) {
    $variables['options']['controls'] = 0;
  }
  else {
    $variables['options']['controls'] = 1;
  }
**/

  // Strings.
/**
  if ($variables['options']['theme'] != 'dark') {
    $query['theme'] = $variables['options']['theme'];
  }
  if ($variables['options']['color'] != 'red') {
    $query['color'] = $variables['options']['color'];
  }
  if ($variables['options']['autohide'] != '2') {
    $query['autohide'] = $variables['options']['autohide'];
  }
**/

  // Non-query options.
//  if ($variables['options']['nocookie']) {
//    $url_base = 'ustream-nocookie.com';
//  }
//  else {
    $url_base = 'ustream.tv';
//  }

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

  // Build the iframe URL with options query string.
  $variables['url'] = url($protocol . '//www.' . $url_base . '/embed/recorded/' . $variables['video_id'], array('query' => $query, 'external' => TRUE));

  // new stuff below
  if ($query['wmode'] == 'direct') {
    $wmode = '<param name="wmode" value="direct"/>';
  }
  elseif ($query['wmode'] == 'opaque') {
    $wmode = '<param name="wmode" value="opaque"/>';
  }
  else {
    $wmode = '';
  }

  if ($variables['options']['allowfullscreen'] == 'true') {
    $allowfullscreen = '<param name="allowfullscreen" value="'.$variables['options']['allowfullscreen'].'"/>';
  }
  else {
    $allowfullscreen = '<param name="allowfullscreen" value="false"/>';
  }

  if ($variables['options']['autoplay'] == 'false') {
    $autoplay = 'false';
  }
  else {
    $autoplay = 'true';
  }

  $variables['wrapper_id'] = 'media_ustream_' . $variables['video_id'] . '_' . $variables['id'];


  // Let's go the vimeo way.
  $variables['output'] = <<<OUTPUT
<object width="{$variables['options']['width']}" height="{$variables['options']['height']}" classid="clsid:d27cdb6e-ae6d-11cf-96b8-444553540000">
$wmode
  <param name="flashvars" value="vid={$variables['video_id']}&amp;autoplay={$autoplay}"/>
$allowfullscreen
  <param name="allowscriptaccess" value="always"/>
  <param name="src" value="http://www.ustream.tv/flash/viewer.swf"/>
  <embed flashvars="vid={$variables['video_id']}&amp;autoplay={$autoplay}" width="{$variables['options']['width']}" height="{$variables['options']['height']}" allowfullscreen="{$variables['options']['allowfullscreen']}" allowscriptaccess="always" src="http://www.ustream.tv/flash/viewer.swf" type="application/x-shockwave-flash"></embed>
</object>
OUTPUT;


  // Pass the settings to replace the object tag with an iframe.
  $settings = array(
    'media_ustream' => array(
      $variables['wrapper_id'] => array(
        'width' => $variables['options']['width'],
        'height' => $variables['options']['height'],
        'video_id' => $variables['video_id'],
        'fullscreen' => $variables['options']['allowfullscreen'],
        'id' => $variables['wrapper_id'] .'_iframe',
      ),
    ),
  );
  if ($variables['options']['autoplay']) {
    $settings['media_ustream'][$variables['wrapper_id']]['options'] = array(
      'autoplay' => $variables['options']['autoplay'],
    );
  }
  drupal_add_js($settings, 'setting');
  drupal_add_js(drupal_get_path('module', 'media_ustream') . '/js/media_ustream.js');
  drupal_add_css(drupal_get_path('module', 'media_ustream') . '/css/media_ustream.css');
  drupal_add_js(drupal_get_path('module', 'media_ustream') . '/js/flash_detect_min.js');
}
