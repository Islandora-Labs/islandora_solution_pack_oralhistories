/**
 * Created by Lingling Jiang on 9/16/14.
 */
(function($){
    Drupal.behaviors.islandoraOralHistories = {
        attach: function (context, settings) {
            //console.dir(Drupal.settings.islandoraOralhistories);
            var targetObjectId = Drupal.settings.islandoraOralhistories.objectId;
            var videoElement = $(".islandora-oralhistories-object").find('video, audio')[0];

            // Show transcript display as tabs.
            $("#transcript-tabs", context).once(function() {
                    $(this).tabs();
                });

        } // end attach function
    };

})(jQuery);




