(function ($) {

    Drupal.behaviors.scrollingTranscript = {
        attach: function (context, settings) {
            $('[data-transcripts-role=transcript]', context)
                .addBack('[data-transcripts-role=transcript]')
                .once('scrolling-transcript')
                .each(function () {
                    var scroller = ScrollingTranscript.getUI($(this));
                    scroller.setContainer($(this).parents('.transcript-container'));
                    scroller.setVideo($('[data-transcripts-role=video][data-transcripts-id=' + $(this).attr('data-transcripts-id') + ']').find('video,audio')[0]);
                });
        }
    }

})(jQuery);
