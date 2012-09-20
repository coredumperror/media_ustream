
/**
 * @file media_ustream/js/media_ustream.js
 */

(function ($) {

Drupal.media_ustream = {};
Drupal.behaviors.media_ustream = {
  attach: function (context, settings) {
    // Check the browser to see if it supports html5 video.
    var video = document.createElement('video');
    var html5 = video.canPlayType ? true : false;

    // If it has video, does it support the correct codecs?
    if (html5) {
      html5 = false;
      if (video.canPlayType( 'video/webm; codecs="vp8, vorbis"' ) || video.canPlayType('video/mp4; codecs="avc1.42E01E, mp4a.40.2"')) {
        html5 = true;
      }
    }

    // Put a prompt in the video wrappers to let users know they need flash
    if (!FlashDetect.installed && !html5){
      $('.media-ustream-preview-wrapper').each(Drupal.media_ustream.needFlash);
    }

    // Replace all object tags with iframes.
    if (Drupal.settings && Drupal.settings.media_ustream) {
      for (video in Drupal.settings.media_ustream) {
        Drupal.media_ustream.insertEmbed(video);
      }
    }
  }
};

Drupal.media_ustream.needFlash = function () {
  var id = $(this).attr('id');
  var wrapper = $('.media-ustream-preview-wrapper');
  var hw = Drupal.settings.media_ustream[id].height / Drupal.settings.media_ustream[id].width;
  wrapper.html('<div class="js-fallback">' + Drupal.t('You need Flash to watch this video. <a href="@flash">Get Flash</a>', {'@flash':'http://get.adobe.com/flashplayer'}) + '</div>');
  wrapper.height(wrapper.width() * hw);
};

Drupal.media_ustream.insertEmbed = function (embed_id) {
  var videoWrapper = $('#' + embed_id + '.media-ustream-preview-wrapper');
  var settings = Drupal.settings.media_ustream[embed_id];

  // Calculate the ratio of the dimensions of the embed.
  settings.hw = settings.height / settings.width;

  // Replace the object embed with UStream's iframe. This isn't done by the
  // theme function because UStream doesn't have a no-JS or no-Flash fallback.
  var video = $('<iframe class="ustream-player" type="text/html" frameborder="0"></iframe>');
  var src = 'http://www.ustream.tv/embed/recorded/' + settings.video_id;

  // Allow other modules to modify the video settings.
  settings.options = settings.options || {};
  $(window).trigger('media_ustream_load', settings);

  // Merge UStream options (such as autoplay) into the source URL.
  var query = $.param(settings.options);
  if (query) {
    src += '?' + query;
  }

  // Set up the iframe with its contents and add it to the page.
  video
    .attr('id', settings.id)
    .attr('width', settings.width)
    .attr('height', settings.height)
    .attr('src', src);
  videoWrapper.html(video);

  // Bind a resize event to handle fluid layouts.
  $(window).bind('resize', Drupal.media_ustream.resizeEmbeds);

  // For some reason Chrome does not properly size the container around the
  // embed and it will just render the embed at full size unless we set this
  // timeout.
  if (!$('.lightbox-stack').length) {
    setTimeout(Drupal.media_ustream.resizeEmbeds, 1);
  }
};

Drupal.media_ustream.resizeEmbeds = function () {
  $('.media-ustream-preview-wrapper').each(Drupal.media_ustream.resizeEmbed);
};

Drupal.media_ustream.resizeEmbed = function () {
  var context = $(this).parent();
  var video = $(this).children(':first-child');
  var hw = Drupal.settings.media_ustream[$(this).attr('id')].hw;
  // Change the height of the wrapper that was given a fixed height by the
  // UStream theming function.
  $(this)
    .height(context.width() * hw)
    .width(context.width());

  // Change the attributes on the embed to match the new size.
  video
    .height(context.width() * hw)
    .width(context.width());
};

})(jQuery);
