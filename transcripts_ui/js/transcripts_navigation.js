(function ($) {
    Drupal.behaviors.transcriptNavigation = {
        attach: function (context, settings) {
            $('[data-transcripts-role=transcript-controls]', context)
                .addBack('[data-transcripts-role=transcript-controls]')
                .once('navigation').each(function () {
                    var trid = $(this).attr('data-transcripts-id');
                    var $transcript = $('[data-transcripts-role=transcript][data-transcripts-id=' + trid + ']');
                    if ($transcript.size() == 1) {
                        var $scroller = ScrollingTranscript.getUI($transcript);
                        $('.previous', this).click(function () {
                            $scroller.previous();
                        });
                        $('.sameagain', this).click(function () {
                            $scroller.sameAgain();
                        });
                        $('.next', this).click(function () {
                            $scroller.next();
                        });
                    }
                });
        }
    }
})(jQuery);
