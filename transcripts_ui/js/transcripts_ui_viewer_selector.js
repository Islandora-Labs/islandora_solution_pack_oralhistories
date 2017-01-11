(function ($) {
    Drupal.behaviors.playerControls = {
        attach: function (context, settings) {
            $('[data-transcripts-role=viewer-selector]', context)
                .addBack('[data-transcripts-role=viewer-selector]')
                .once('transcripts')
                .each(function () {
                    var trid = $(this).attr('data-transcripts-id');
                    var $player = $('#' + trid);

                    var $viewerSelect = $('.viewer-select', this);
                    $viewerSelect.val($player.attr('data-defaultviewer'))
                        .data('oldViewer', $viewerSelect.val())
                        .change(
                        function () {
                            var oldViewer = $(this).data('oldViewer');
                            var goodbye = Drupal.settings['goodbye'][oldViewer];
                            if (goodbye != '') {
                                var fn = window[goodbye];
                                if (typeof fn === 'function') {
                                    fn($player);
                                }
                            }
                            var newViewer = $(this).val();
                            var hello = Drupal.settings['hello'][newViewer];
                            if (hello != '') {
                                var fn = window[hello];
                                if (typeof fn === 'function') {
                                    fn($player);
                                }
                            }
                            $(this).data('oldViewer', newViewer);
                            $(this).blur();
                        }
                    );
                });
        }
    }
})(jQuery);
