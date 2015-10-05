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
            // transcript edit dialog
            var dialog, form;
            dialog = $("#transcript-edit-dialog").css("display", "inline-block").dialog({
                autoOpen: false,
                height: window.innerHeight * 0.7,
                minHeight: 300,
                width: window.innerWidth * 0.7,
                minWidth: 350,
                modal: true,
                buttons: {
                    "Save" : function(){
                        $(this).find("form").submit();
                        $(this).dialog("close");
                    },
                    Cancel: function() { $(this).dialog("close");}
                }
            });
            form = dialog.find( "form" ).on( "submit", function( event ) {
                event.preventDefault();
                saveTranscript();
            });
            function saveTranscript() {console.log("submit transcript")}
            $("#transcript-edit", context).click(function(){
                dialog.dialog("open");
            });

        } // end attach function

    };

})(jQuery);


