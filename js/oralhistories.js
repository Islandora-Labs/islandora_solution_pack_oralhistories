/**
 * Created by Lingling Jiang on 9/16/14.
 */
(function($){
    var transcripts = [];
    //var myPlayer = videojs('islandora_videojs');
    Drupal.behaviors.islandoraOralHistories = {
        attach: function (context, settings) {
            var enableCaption = Drupal.settings.islandora_oralhistories.enbableTranscriptDisplay;
            var racksUploaded = Drupal.settings.islandora_oralhistories.tracks;
            var enableTranscriptDisp = Drupal.settings.islandora_oralhistories.enbableTranscriptDisplay;
            var oVideo = $('.islandora-oralhistories-content', context).find('video');
            var oTrackList = oVideo[0].textTracks;
            var oCues = oTrackList[0].cues;
            var transcriptContent = $('#transcript-content');
            var transcripts = [];
            console.log(oVideo[0]);



            oVideo.on('loadedmetadata', function(){

                if (enableTranscriptDisp) {
                    // video.js throw errors when kind="metadata"
                    // codes to solve goes here...
                    for (i = 0; i<oTrackList.length; i++) {
                        switch (oTrackList[i].mode) {
                            case 0:
                                oTrackList[i].mode = 1;
                                break;
                            case 'disabled':
                                oTrackList[i].mode = 'showing';
                                break;
                        }
                    }
                } //end if (enbableTranscriptDisp)
            });



            // Sync transcript display with media.
            transcriptContent.on("click",function(e) {
                if(e.target.id.indexOf("transcript") == 0) {
                    var i = Number(e.target.id.replace("transcript",""));
                    oVideo[0].currentTime = $('#transcript' + i).attr('data-start-time');
                    oVideo[0].play();
                }
            });





        } // end attach function

    };

})(jQuery);


