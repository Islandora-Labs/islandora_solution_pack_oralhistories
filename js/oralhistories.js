/**
 * Created by Lingling Jiang on 9/16/14.
 */
(function($){
    var transcripts = [];
    //var myPlayer = videojs('islandora_videojs');
    Drupal.behaviors.islandoraOralHistories = {
        attach: function (context, settings) {
            var enableCaption = Drupal.settings.islandora_oralhistories.enbableTranscriptDisplay;
            var tracksUploaded = Drupal.settings.islandora_oralhistories.tracks;
            var enableTranscriptDisp = Drupal.settings.islandora_oralhistories.enbableTranscriptDisplay;
            var oVideo = $('.islandora-oralhistories-content video', context);

            oVideo.on('loadedmetadata', function(){

                if (enableTranscriptDisp) {

                    // video.js throw errors when kind="metadata"
                    // codes to solve goes here...
                    /*
                     for (i = 0; i<myTextTracks.length; i++) {
                     switch (myTextTracks[i].mode) {
                     case 0:
                     myTextTracks[i].mode = 1;
                     break;
                     case 'disabled':
                     myTextTracks[i].mode = 'showing';
                     break;
                     }
                     }

                     */

                    //loadTranscripts();

                } //end if (enbableTranscriptDisp)
            });



        } // end attach function

    };




    // Helper function.
    function loadTranscripts(){
        var oTrack = document.getElementById('track0');
        var oTextTrack = oTrack.track;
        var oCueList = oTextTrack.cues;
        var transHtml = '';
        for (i = 0; i < oCueList.length; i++) {
            var sText = oCueList[i].text;        // get text from first cue
            var sTime = oCueList[i].startTime;   // get start time from first cue
            var eTime = oCueList[i].endTime;
            transHtml += '<p><span id="transcript-line' + i + '" data-start-time="' + sTime + '" ';
            transHtml += 'data-end-time="' + eTime + '">' + sText + '</span></p>';
        }

        $('#transcript-content').append(transHtml);
    };



})(jQuery);


