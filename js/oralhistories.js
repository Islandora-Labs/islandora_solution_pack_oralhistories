/**
 * Created by Lingling Jiang on 9/16/14.
 */
(function($){
    Drupal.behaviors.islandoraOralHistories = {
        attach: function (context, settings) {
            var targetObjectId = Drupal.settings.islandoraOralhistories.objectId;

            // Show transcript display as tabs.
            $("#transcript-tabs", context).once(function() {
                    $(this).tabs();
                });




        } // end attach function
    };
})(jQuery);


