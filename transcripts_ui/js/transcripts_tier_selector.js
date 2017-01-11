(function ($) {
    Drupal.behaviors.tierSelector = {
        attach: function (context, settings) {
            $('[data-transcripts-role=transcript-controls]', context)
                .addBack('[data-transcripts-role=transcript-controls]')
                .once('tier-selector')
                .each(function () {
                    var trid = $(this).attr('data-transcripts-id');

                    var $tierSelector = $('.tier-selector', this);
                    $tierSelector.find('optgroup[data-type=languages] option').attr('selected', true);
                    $tierSelector.find('optgroup[data-type=speakers] option').attr('selected', true)
                    $tierSelector.change(function (e) {
                            //language selection
                            $('*[data-transcripts-id=' + trid + ']').find('.tier').hide(); //removeClass('active');
                            $('optgroup[data-type=languages] option:selected', this).each(function () {
                                $('*[data-transcripts-id=' + trid + ']').find('*[data-tier=' + $(this).val() + ']').show(); //addClass('active');
                            });

                            //speaker name selection
                            $('*[data-transcripts-id=' + trid + ']').find('.speaker-display').hide(); //removeClass('active');
                            $('optgroup[data-type=speakers] option:selected', this).each(function () {
                                $('*[data-transcripts-id=' + trid + ']').find('*[data-speaker-display=' + $(this).val() + ']').show(); //addClass('active');
                            });
                            e.preventDefault();
                        }
                    );

                    //hide buttons for tiers that have no data
                    $('optgroup[data-type=languages] option', $tierSelector).each(function () {
                        if ($('*[data-transcripts-id=' + trid + ']').find('*[data-tier=' + $(this).val() + ']').size() == 0) {
                            $(this).remove();
                        }
                    });

                    //hide buttons for speaker name formats that have no data
                    $('optgroup[data-type=speakers] option', $tierSelector).each(function () {
                        if ($('*[data-transcripts-id=' + trid + ']').find('*[data-speaker-display=' + $(this).val() + ']').size() == 0) {
                            $(this).remove();
                        }
                    });
                });
        }
    };
})(jQuery);
