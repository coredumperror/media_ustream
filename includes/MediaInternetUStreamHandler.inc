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

  /**
   * Check if a UStream video id is valid.
   *
   * @return
   *   Boolean.
   */
  static public function validId($id) {
    $video_properties = self::getVideoProperties($id);
    // Check if the result contains an error.
    if ($video_properties['error'] != FALSE) {
      throw new MediaInternetValidationException('The UStream video ID is invalid, is set to private or has been deleted.');
    }
    return TRUE;
  }

  public function parse($embedCode) {
    $patterns = array(
      '@ustream\.tv/recorded/(\d+)@i',
      '@ustream\.tv/embed/recorded/([^"\&\?]+)@i',
      '@flashvars="loc=%2F&amp;autoplay=false&amp;vid=(\d+)@i',
    );
    foreach ($patterns as $pattern) {
      preg_match($pattern, $embedCode, $matches);
      // @TODO: Parse is called often. Refactor so that valid ID is checked
      // when a video is added, but not every time the embedCode is parsed.

      // @TODO: check if this is also valid for the vimeo-way we use.

      if (isset($matches[1]) && self::validId($matches[1])) {

//    $video_properties = self::getVideoProperties($matches[1]);
//        return file_stream_wrapper_uri_normalize('ustream://v/' . $video_properties['results']['title']);
        return file_stream_wrapper_uri_normalize('ustream://v/' . $matches[1]);
      }
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


//    $video_properties = self::getVideoProperties($file->filename);
//print_r($video_properties);

    if (empty($file->fid) && $info = $this->getOEmbed()) {
//      $file->filename = truncate_utf8($video_properties['results']['title'], 255);
      $file->filename = truncate_utf8($info['title'], 255);
    }
    return $file;
  }

  /**
   * Returns information about the media. See http://video.search.yahoo.com/mrss.
   *
   * @return
   *   If ATOM+MRSS information is available, a SimpleXML element containing
   *   ATOM and MRSS elements, as per those respective specifications.
   *
   * @todo Would be better for the return value to be an array rather than a
   *   SimpleXML element, but media_retrieve_xml() needs to be upgraded to
   *   handle namespaces first.
   */
//  public function getMRSS() {
//    $uri = $this->parse($this->embedCode);
//    $video_id = arg(1, file_uri_target($uri));
//    $rss_url = url('http://ustream.tv/feeds/api/videos/' . $video_id, array('query' => array('v' => '2')));
//    // @todo Use media_retrieve_xml() once it's upgraded to include elements
//    //   from all namespaces, not just the document default namespace.
//    $request = drupal_http_request($rss_url);
//    if (!isset($request->error)) {
//      $entry = simplexml_load_string($request->data);
//    }
//    else {
//      throw new Exception("Error Processing Request. (Error: {$response->code}, {$response->error})");
//
//      //if request wasn't successful, create object for return to avoid errors
//      $entry = new SimpleXMLElement();
//    }
//    return $entry;
//  }

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
    $external_url = file_create_url($uri);
    $oembed_url = url('http://www.ustream.tv/', array('query' => array('url' => $external_url, 'format' => 'json')));
    $response = drupal_http_request($oembed_url);
    if (!isset($response->error)) {
      return drupal_json_decode($response->data);
    }
    else {
      throw new Exception("Error Processing Request. (Error: {$response->code}, {$response->error})");
      return;
    }
  }
}
