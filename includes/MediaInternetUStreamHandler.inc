<?php

/**
 * @file
 * Contains MediaInternetUStreamHandler.
 */

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers().
 */
class MediaInternetUStreamHandler extends MediaInternetBaseHandler {
  
  /**
   * Call the UStream API to fetch the video information.
   *
   * See http://developer.ustream.tv/data_api/docs
   *
   * @return
   *   Array of properties.
   */
  static public function getVideoProperties($id) {
    // e.g. http://api.ustream.tv/json/video/38492/getInfo
    $response = drupal_http_request(MEDIA_USTREAM_REST_API . '/json/video/' . $id . '/getInfo');
    return drupal_json_decode($response->data);
  }
  
  /**
   * Call the UStream API to fetch the channel information.
   *
   * See http://developer.ustream.tv/data_api/docs
   *
   * @return
   *   Array of properties.
   */
  static public function getChannelProperties($id) {
    // e.g. http://api.ustream.tv/json/channel/api-test-show/getInfo
    $response = drupal_http_request(MEDIA_USTREAM_REST_API . '/json/channel/' . $id . '/getInfo');
    return drupal_json_decode($response->data);
  }
  
  /**
   * Check if a UStream video/channel id is valid.
   *
   * @param $id - The video or channel ID.
   * @param $type - A string, either 'live' or 'recorded'.
   *
   * @return
   *   Boolean.
   *
   * @throws
   *   MediaInternetValidationException - When the ID is invalid.
   */
  static public function validId($id, $type) {
    if ($type == 'recorded') {
      $video_properties = self::getVideoProperties($id);
      if ($video_properties['error']) {
        throw new MediaInternetValidationException(
            "The UStream video ID '$id' does not exist, is set to private, or has been deleted.");
      }
      return $video_properties;
    }
    else if ($type == 'live') {
      $channel_properties = self::getChannelProperties($id);
      if ($channel_properties['error'] || $channel_properties['results']['title'] == NULL) {
        // $channel_properties['error'] is non-NULL when referring to a channel by a bad name string.
        // The Title is NULL when referring to a channel by a bad id (this is probably an API bug).
        throw new MediaInternetValidationException(
            "The UStream channel ID '$id' does not exist, is set to private, or has been deleted.");
      }
      return $channel_properties;
    }
    return FALSE;
  }
  
  /**
   * Check if a UStream id is already stored in the DB.
   *
   * @param $id - The video or channel ID.
   * @param $type - A string, either 'live' or 'recorded'.
   *
   * @return
   *   Boolean.
   */
  static public function knownId($id, $type) {
    $file_query = new EntityFieldQuery;
    $results = $file_query->entityCondition('entity_type', 'file')
      ->propertyCondition('uri', "ustream://$type/$id")
      ->execute();
    
    return (bool)$results;
  }
  
  public function parse($embedCode) {
    $embedCode = trim($embedCode);
    
    // First, check if it's a recorded video pattern.
    $recorded_patterns = array(
      '@ustream\.tv/recorded/(\d+)@i', // Matches the URL for the video page.
      '@ustream\.tv/embed/recorded/([^"\&\?]+)@i', // Matches the URL from the embed code for recorded videos.
    );
    foreach ($recorded_patterns as $pattern) {
      preg_match($pattern, $embedCode, $matches);
      // If a match was found, check if it's a video that's already saved locally, before calling the API to see
      // if it's a valid video id.
      if (isset($matches[1]) && (self::knownId($matches[1], 'recorded') || self::validId($matches[1], 'recorded'))) {
        return file_stream_wrapper_uri_normalize('ustream://recorded/' . $matches[1]);
      }
    }
    
    // If it's not a recorded video, check if it's a live channel.
    $matches = array();
    $match = '';
    preg_match('@ustream\.tv/channel/([a-zA-Z\-]+)@i', $embedCode, $matches);
    if (isset($matches[1])) {
      // This is a channel URL (as opposed to an embed code), so we need to convert the contained
      // channel name into a channel ID, since all our ustream:// uris use IDs.
      $properties = self::getChannelProperties($matches[1]);
      if (!$properties['error']) {
        $match = $properties['results']['id'];
      }
    }
    // If the channel name didn't match, check if it's a live channel embed code.
    if (!$match) {
      preg_match('@ustream\.tv/embed/([^"\&\?]+)@i', $embedCode, $matches);
      if (isset($matches[1]) && (self::knownId($matches[1], 'live') || self::validId($matches[1], 'live'))) {
        $match = $matches[1];
      }
    }
    
    if ($match) {
      return file_stream_wrapper_uri_normalize('ustream://live/' . $match);
    }
  }
  
  public function claim($embedCode) {
    if ($this->parse($embedCode)) {
      return TRUE;
    }
  }
  
  public function getFileObject() {
    $uri = $this->parse($this->embedCode);
    $file = file_uri_to_object($uri, TRUE);
    
    if (empty($file->fid) && $info = $this->getOEmbed()) {
      $file->filename = truncate_utf8($info['title'], 255);
    }
    
    return $file;
  }
  
  /**
   * Returns information about the media. See http://www.oembed.com/.
   *
   * @return
   *   If oEmbed information is available, an array containing 'title', 'type',
   *   'url', and other information as specified by the oEmbed standard.
   *   Otherwise, NULL.
   */
  public function getOEmbed() {
    $uri = $this->parse($this->embedCode);
    $video_url = file_create_url($uri);
    $oembed_url = url('http://www.ustream.tv/oembed', array('query' => array('url' => $video_url, 'format' => 'json')));
    $response = drupal_http_request($oembed_url);
    if (!isset($response->error)) {
      return drupal_json_decode($response->data);
    }
    else {
      throw new Exception("Error Processing Request. (Error: {$response->code}, {$response->error})");
    }
  }
}
