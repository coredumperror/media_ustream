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
   * Call the UStream API to fetch the user information.
   *
   * See http://developer.ustream.tv/data_api/docs
   *
   * @return
   *   Array of properties.
   */
  static public function getUserProperties($id) {
    // e.g. http://api.ustream.tv/json/user/[username]/getInfo
    $response = drupal_http_request(MEDIA_USTREAM_REST_API . '/json/user/' . $id . '/getInfo');
    return drupal_json_decode($response->data);
  }
  static public function getUserID($username) {
    // e.g. http://api.ustream.tv/json/user/[username]/getInfo
    $response = drupal_http_request(MEDIA_USTREAM_REST_API . '/json/user/' . $username . '/getUserId');
    return drupal_json_decode($response->data);
  }
  static public function getUserChannels($id) {
    // e.g. http://api.ustream.tv/json/user/[username]/getInfo
    $response = drupal_http_request(MEDIA_USTREAM_REST_API . '/json/user/' . $id . '/listAllChannels');
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
    switch ($type) {
      case 'recorded':
        $properties = self::getVideoProperties($id);
        if ($properties['error']) {
          throw new MediaInternetValidationException(
              "The UStream video ID '$id' does not exist, is set to private, or has been deleted.");
        }
        break;

      case 'live':
        $properties = self::getChannelProperties($id);
        if ($properties['error'] || $properties['results']['title'] == NULL) {
          // $channel_properties['error'] is non-NULL when referring to a channel by a bad name string.
          // The Title is NULL when referring to a channel by a bad id (this is probably an API bug).
          throw new MediaInternetValidationException(
              "The UStream channel ID '$id' does not exist, is set to private, or has been deleted.");
        }
        break;

      case 'user':
        $properties = self::getUserProperties($id);
        if ($properties['error']) {
          throw new MediaInternetValidationException(
              "The UStream user ID '$id' does not exist, is set to private, or has been deleted.");
        }
        break;

      case 'channeloruser':
        // Check if this is a channel.
        $properties = self::getChannelProperties(check_plain($id));
        // If we have an error, it isn't a channel and
        // we need to check for user channels.
        if (!empty($properties['error'])) {
          // Grab the clean oembed.
          $oembed = self::getViaOEmbed(check_plain($id));
          // Unknown user and channel, we will abort.
          if (!empty($oembed['error'])) {
            throw new MediaInternetValidationException(
              "The UStream user or channel ID '$id' does not exist, is set to private, or has been deleted.");
            return FALSE;
          }
          // We need the last part of the author_url.
          $path = explode('/user/', $oembed['author_url']);
          // If this is not a channel, check if it's a user with channels.
          $user = self::getUserProperties($path[1]);
          // If it indeed IS a known user, continue, else trow an error.
          if (empty($user['error'])) {
            // Fetch user channels.
            $channels = self::getUserChannels(check_plain($user['results']['id']));
            // Then fetch the user channel details.
            if (!$channels['error']) {
              // Search the channels for the current channel title.
              foreach($channels['results'] as $key => $channel) {
                if ($oembed['title'] == $channel['title']) {
                  $properties = self::getChannelProperties(check_plain($channels['results'][$key]['id']));
                  // Add this as a value, because [0] wont do.
                  $properties['results']['title'] = check_plain($channel['title']);
                  break;

                }
              }
            }
          }
          // Don't think this is ever called, because you need at least one (1)
          // channel as a user, but still not sure, so i'll keep it in for now.
          else {
            throw new MediaInternetValidationException(
              "The UStream user or channel ID '$id' does not exist, is set to private, or has been deleted.");
          }
        }
        break;

    }

    // If we have a result, return it, else return false.
    if (!empty($properties)) {
      return $properties;
    }
    else {
      // No match = false.
      return FALSE;
    }
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
    if ($type == 'channeloruser') {
      // TODO: PDO-dify this. (can EntityFieldQuery do this?)
      $result = db_query("SELECT fid FROM {file_managed} WHERE REPLACE(filename, ' ', '') = '$id'");
      $data = $result->fetchAll();
      if (!empty($data[0])) {
        return $data[0]->fid;
      }
      return FALSE;
    }

    $file_query = new EntityFieldQuery;
    $result = $file_query->entityCondition('entity_type', 'file')
      ->propertyCondition('uri', "ustream://$type/$id")
      ->execute();

    if (!empty($result['file'])) {
      return key($result['file']);
    }
    return FALSE;

  }

  /**
   * Public function preparse().
   *
   * Created to save some time.
   */
  public function preparse($embedCode) {
    $recorded_patterns = array(
      '@ustream\.tv/recorded/(\d+)@i',             // Matches the URL for the video page.
      '@ustream\.tv/embed/recorded/([^"\&\?]+)@i', // Matches the URL from the embed code for recorded videos.
      '@ustream\.tv/channel/([a-zA-Z0-9_\-]+)@i',  // channel.
      '@ustream\.tv/embed/([^"\&\?]+)@i',          // channel embed.
      '@ustream\.tv/user/([a-zA-Z0-9_\-]+)@i',     // Matches the URL for the user page.
      '@ustream\.tv/([a-zA-Z0-9_\-]+)@i',          // channel or user
    );
    foreach ($recorded_patterns as $pattern) {
      preg_match($pattern, $embedCode, $matches);
      if (!empty($matches[1])) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Public function parse().
   */
  public function parse($embedCode) {
    $embedCode = trim($embedCode);

    // Patterns.
    $recorded_patterns = array(
      '@ustream\.tv/recorded/(\d+)@i'             => 'recorded',      // Matches the URL for the video page.
      '@ustream\.tv/embed/recorded/([^"\&\?]+)@i' => 'recorded',      // Matches the URL from the embed code for recorded videos.
      '@ustream\.tv/channel/([a-zA-Z0-9_\-]+)@i'  => 'live',          // Channel.
      '@ustream\.tv/embed/([^"\&\?]+)@i'          => 'live',          // Channel embed.
      '@ustream\.tv/user/([a-zA-Z0-9_\-]+)@i'     => 'user',          // Matches the URL for the user page.
      '@ustream\.tv/([a-zA-Z0-9_\-]+)@i'          => 'channeloruser', // User channel or promoted channel.
    );

    foreach ($recorded_patterns as $pattern => $case) {
      preg_match($pattern, $embedCode, $matches);
      // If a match was found, check if it's a video that's already saved,
      // locally before calling the API to see if it's a valid video id.
      if (!empty($matches[1])) {
        if ($fid = self::knownId(check_plain($matches[1]), $case)) {
          // No need to do anything, we already know this $embedCode.
          // Grab the file id, load the file and return normalized string.
          $existing_file = file_load($fid);
          $parts = explode('://', $existing_file->uri);
          $file_uri = $parts[1];
        }
        else {
          $properties = self::validId(check_plain($matches[1]), $case);
          $this->title = $properties['results']['title'];
          $file_uri = $case . '/' . check_plain($properties['results']['id']);
          if ($case == 'channeloruser') {
            $file_uri = 'live/' . check_plain($properties['results']['id']);
          }
        }
        // If we have a $file_uri, Use it.
        if (!empty($file_uri)) {
          return file_stream_wrapper_uri_normalize('ustream://' . $file_uri);
        }
      }
    }
  }

  /**
   * Function claim().
   */
  public function claim($embedCode) {
    // Preparse is the same as parse, but it only returns
    // a bool true/false and does not return a file.
    // I've done this to save speed. (and it's a lot).
    if (empty($this->fileObject->fid) && $this->preparse($embedCode)) {
      return TRUE;
    }
  }

  /**
   * Function getFileObject().
   */
  public function getFileObject() {
    if (empty($this->fileObject)) {
      $uri = $this->parse($this->embedCode);
      $this->fileObject = file_uri_to_object($uri, TRUE);
    }
    if (empty($this->fileObject->fid) && !empty($this->title)) {
      $this->fileObject->filename = truncate_utf8($this->title, 255);
    }
    return $this->fileObject;
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
    // Deprecated in favour of the JSON API. (+ it isnt really working that good for us).
    return FALSE;
  }

  // I've created this functions, because getOEmbed() is
  // abstracted and we might use it later on again (original state) and
  // we call getViaOEmbed from parse(), so that's the other way arround.
  // We only need this to map channels to user accounts if
  //the channel name != the user name, that's why we need this.
  // See parse() for details.
  static public function getViaOEmbed($uri) {
    $oembed_url = 'http://www.ustream.tv/oembed?url=http%3A//ustream.tv/' . $uri . '&format=json';

    $response = drupal_http_request($oembed_url);
    if (empty($response->error)) {
      return drupal_json_decode($response->data);
    }
    else {
      throw new Exception("Error Processing Request. (Error: {$response->code}, {$response->error})");
    }
  }
}
