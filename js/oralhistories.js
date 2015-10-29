/**
 * Created by Lingling Jiang on 9/16/14.
 */
(function($){
    Drupal.behaviors.islandoraOralHistories = {
        attach: function (context, settings) {
            //var enableCaptionDisp = Drupal.settings.islandoraOralhistories.enbableCaptionDisplay;
            //var tracksUploaded = Drupal.settings.islandoraOralhistories.tracks;
            var targetObjectId = Drupal.settings.islandoraOralhistories.objectId;

            // Show transcript display as tabs.
            $("#transcript-tabs", context).once(function() {
                    $(this).tabs();
                });


            // Annotator
            var videoElement = $(".islandora-oralhistories-object").find('video, audio')[0];
            //console.log(videoElement);
            var vjsPlayer = videojs(videoElement);
            console.log(vjsPlayer);
            vjsPlayer.annotator({target: targetObjectId});



        } // end attach function
    };
})(jQuery);


