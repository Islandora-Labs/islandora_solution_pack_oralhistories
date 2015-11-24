/**
 * Created by Lingling Jiang on 9/16/14.
 */
(function($){
    Drupal.behaviors.islandoraOralHistories = {
        attach: function (context, settings) {
            //console.dir(Drupal.settings.islandoraOralhistories);
            var targetObjectId = Drupal.settings.islandoraOralhistories.objectId;
            var enableAnnotation = Drupal.settings.enableAnnotationTabDisplay;
            var videoElement = $(".islandora-oralhistories-object").find('video, audio')[0];

            // Show transcript display as tabs.
            $("#transcript-tabs", context).once(function() {
                    $(this).tabs();
                });

            // Annotation.
            //var annotationForm = $("#islandora-oralhistories-annotation-form");
            //var annotationFormStart = $("#web-annotation-start-time");
            //var annotationFormSubmit = $("#edit-web-annotation-submit");
            //var addNewAnnotation = $("#annotation-add-new-btn");
            //
            //addNewAnnotation.once("load-web-annotation-form", function(){
            //    var ajaxLoadUrl = '/islandora/object/' + targetObjectId + '/web_annotation/add';
            //    $(this).click(function(){
            //        $("#web-annotation-form-ajax-loader").load(ajaxLoadUrl, function(){
            //            Drupal.attachBehaviors();
            //            if (!videoElement.paused) {
            //                videoElement.pause();
            //                $("#web-annotation-start-time").val(videoElement.currentTime.toFixed(2));
            //            }
            //        });
            //
            //    });
            //});



        } // end attach function
    };

})(jQuery);




