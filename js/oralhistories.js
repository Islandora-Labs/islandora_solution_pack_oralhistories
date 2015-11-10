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
            var annotationForm = $("#islandora-oralhistories-annotation-form");
            var annotationFormStart = $("#web-annotation-start-time");
            var annotationFormSubmit = $("#edit-web-annotation-submit");
            var addNewAnnotation = $("#web-annotation-add-new");
            addNewAnnotation.change(function(){
                if (this.checked) {
                    if (!videoElement.paused) {
                        videoElement.pause();
                        annotationFormStart.val(videoElement.currentTime.toFixed(2));
                    }
                }
            });
            //annotationForm.submit(function() {
            //    this.submit();
            //    if (annotationFormSubmit.hasClass("ajax-processed")) {
            //        $("#web-annotation-container-wrapper").hide();
            //    }
            //});



        } // end attach function
    };
})(jQuery);


