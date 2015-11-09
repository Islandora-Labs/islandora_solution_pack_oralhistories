/**
 * Created by Lingling Jiang on 9/16/14.
 */
(function($){
    Drupal.behaviors.islandoraOralHistories = {
        attach: function (context, settings) {
            var targetObjectId = Drupal.settings.islandoraOralhistories.objectId;
            var enableAnnotation = Drupal.settings.enableAnnotationTabDisplay;
            var videoElement = $(".islandora-oralhistories-object").find('video, audio')[0];

            // Show transcript display as tabs.
            $("#transcript-tabs", context).once(function() {
                    $(this).tabs();
                });

            // Annotation.
            var addNewAnnotation = $("#add-new-annotation");
            addNewAnnotation.change(function(){
                if (this.checked) {
                    console.log("The addnewannotation is checked.");
                }
            });


        } // end attach function
    };
})(jQuery);


