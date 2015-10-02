/**
 * Created by Lingling Jiang on 9/16/14.
 */
(function($){
    Drupal.behaviors.islandoraOralHistories = {
        attach: function (context, settings) {
            //var enableCaptionDisp = Drupal.settings.islandoraOralhistories.enbableCaptionDisplay;
            //var tracksUploaded = Drupal.settings.islandoraOralhistories.tracks;

            // Show transcript display as tabs.
            $("#transcript-tabs", context).once(function() {
                    $(this).tabs();
                });


        } // end attach function

    };

})(jQuery);


