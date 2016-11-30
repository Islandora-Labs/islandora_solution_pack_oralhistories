/**
 * Created by edwardjgarrett on 1/29/15.
 */
(function ($) {

    //http://drupal.stackexchange.com/questions/79521/invoke-custom-js-function-in-ajax-callback
    Drupal.behaviors.transcriptSearch = {
        attach: function (context, settings) {
            $('[data-transcripts-role=transcript-search]', context)
                .addBack('[data-transcripts-role=transcript-search]')
                .once('transcript-search')
                .each(function () {
                    var $form = $(this);
                    var trid = $form.attr('data-transcripts-id');
                    var $transcript = $('[data-transcripts-role=transcript][data-transcripts-id=' + trid + ']');
                    var $scroller = ScrollingTranscript.getUI($transcript);
                    if (settings.hasOwnProperty('hasSearched') && settings.hasSearched) $form.addClass('has-searched');
                    var hitCount = settings.hasOwnProperty('hitCount') ? settings.hitCount : 0;
                    $('#transcript-results-count-' + trid).attr('data-results-count', hitCount);

                    var hitIndex = 1;
                    var scrollHit = function (i) {
                        var $tcu = $('li[data-tcuid]:has(.hit):eq(' + (i - 1) + ')');
                        $scroller.endAll();
                        $scroller.setOne($tcu);
                        $scroller.container.scrollTo($tcu);
                        $('#transcript-results-count-' + trid).html(i + ' of ' + hitCount);
                    };

                    var clearHits = function () {
                        $('.hit', $transcript).removeClass('hit').find('mark').contents().unwrap();
                    };

                    $('input:radio[name=transcript-search-options]', $form).click(function () {
                        if ($(this).attr('data-value') == 0) {
                            $('li[data-tcuid]', $transcript).show();
                            if (hitCount > 0) {
                                scrollHit(hitIndex);
                            }
                        }
                        else {
                            $('li[data-tcuid]:not(:has(.hit))').hide();
                            if (hitCount > 0) {
                                scrollHit(hitIndex);
                            }
                        }
                    });
                    $('input:radio[name=transcript-search-options]', $form).first().click();

                    if (hitCount > 0) {
                        $('#transcript-nextresult-' + trid).click(function () {
                            if (hitIndex < hitCount) {
                                hitIndex++;
                                scrollHit(hitIndex);
                            }
                        });
                        $('#transcript-previousresult-' + trid).click(function () {
                            if (hitIndex > 1) {
                                hitIndex--;
                                scrollHit(hitIndex);
                            }
                        });

                        scrollHit(hitIndex);
                    }

                    var $reset = $('button[type=reset]', $form);

                    var $mbsrch = $('input[name=term]', $form); // the main search input
                    $mbsrch.data('holder', $mbsrch.attr('placeholder'));

                    var term;
                    if ($form.hasClass('has-searched')) {
                        term = $mbsrch.val();
                        $reset.show();
                    }
                    else {
                        term = '';
                        $mbsrch.val('');
                    }

                    // --- focusin - focusout
                    $mbsrch.focusin(function () {
                        $mbsrch.attr('placeholder', '');
                        $reset.show("fast");
                    });

                    $mbsrch.focusout(function () {
                        $mbsrch.attr('placeholder', $mbsrch.data('holder'));
                        $reset.hide();

                        if ($mbsrch.val() == '') {
                            if ($form.hasClass('has-searched')) { //empty so return to search term
                                $mbsrch.val(term);
                                $reset.show();
                                return false;
                            }
                            else { //empty so keep cancel button hidden
                                return true;
                            }
                        }
                        else { //non-empty search so show cancel button
                            $reset.show();
                            return false;
                        }
                    });

                    $reset.click(function (e) {
                        $('li[data-tcuid]', $transcript).show(); //show entire transcript
                        $form.removeClass('has-searched');
                        $mbsrch.val('');
                        $scroller.endAll();
                        $(this).hide();
                        clearHits();
                        e.preventDefault();
                    });

                    // Overwrite beforeSubmit
                    Drupal.ajax['transcript-search-button-' + trid].options.beforeSubmit = function (form_values, element, options) {
                        clearHits();
                        $scroller.endAll();
                        $form.addClass('searching');
                    };
                });
        }
    };
})(jQuery);

