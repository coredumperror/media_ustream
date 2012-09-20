<?php

/**
 *  @file
 *  Create a UStream Stream Wrapper class for the Media/Resource module.
 */

/**
 *  Create an instance like this:
 *  $ustream = new MediaUStreamStreamWrapper('ustream://v/[video-code]');
 */
class MediaUStreamStreamWrapper extends MediaReadOnlyStreamWrapper {

  // Overrides $base_url defined in MediaReadOnlyStreamWrapper.
  protected $base_url = 'http://www.ustream.tv/recorded';

  static function getMimeType($uri, $mapping = NULL) {
    return 'video/ustream';
  }

  /**
   * Call the UStream API and fetch the video information.
   *
   * See http://developer.ustream.tv/data_api/docs
   *
   * @return
   *   Array of properties.
   */
  static function getVideoProperties($id) {
    // The .php format returns a serialized array.
    $response = drupal_http_request('http://api.ustream.tv/php/video/' . $id . '/getInfo');
    // @TODO: change this to work with drupal_json_decode().

    return unserialize($response->data);
  }


  function getTarget($f) {
    return FALSE;
  }

  /**
   * Returns a url in the format "http://ustream.tv/recorded/123456".
   *
   * Overrides interpolateUrl() defined in MediaReadOnlyStreamWrapper.
   * This is an exact copy of the function in MediaReadOnlyStreamWrapper,
   * here in case that example is redefined or removed.
   */
  function interpolateUrl() {
    if ($parameters = $this->get_parameters()) {
      return $this->base_url . '/' . $parameters['v'];
    }
  }


  function getOriginalThumbnailPath() {
    $video_properties = self::getVideoProperties($this->parameters['v']);
    return $video_properties['results']['imageUrl']['medium'];
  }

  function getLocalThumbnailPath() {
    $parts = $this->get_parameters();

    $local_path = 'public://media-ustream/' . check_plain($parts['v']) . '.jpg';
    if (!file_exists($local_path)) {
      $dirname = drupal_dirname($local_path);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      $response = drupal_http_request($this->getOriginalThumbnailPath());
      if (!isset($response->error)) {
        file_save_data($response->data, $local_path, TRUE);
      }
      else {
        @copy($this->getOriginalThumbnailPath(), $local_path);
      }
    }
    return $local_path;
  }
}
