/**
 * Created by Lingling Jiang on 9/16/14.
 */
(function($){
    var transcripts = [];
    Drupal.behaviors.islandoraOralHistories = {
        attach: function (context, settings) {
            var enableCaptionDisp = Drupal.settings.islandora_oralhistories.enbableCaptionDisplay;
            var tracksUploaded = Drupal.settings.islandora_oralhistories.tracks;
            var enableTranscriptDisp = Drupal.settings.islandora_oralhistories.enbableTranscriptDisplay;
            var oVideo = $('.islandora-oralhistories-content', context).find('video');
            var oTrackList = oVideo[0].textTracks;
            var transcripts = [];
            var myPlayer = oVideo[0];


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
            $('#transcript-content').on("click",function(e) {
                if(e.target.id.indexOf("transcript") == 0) {
                    var i = Number(e.target.id.replace("transcript",""));
                    myPlayer.currentTime = $('#transcript' + i).attr('data-start-time');
                    myPlayer.play();
                }
            });



            myPlayer.addEventListener('timeupdate', function(){
                transcripts = $('#transcript-content').find('span[id^="transcript"]');
                var curTime = myPlayer.currentTime;
                for (i = 0; i<transcripts.length; i++) {
                    var begin = $('#transcript'+i).attr('data-start-time');
                    var end = $('#transcript'+i).attr('data-end-time');
                    if (curTime > begin && curTime < end) {
                        if (!($('#transcript'+i).hasClass('current'))) {
                            $('#transcript'+i).addClass('current');
                            var height = document.getElementById('transcript-content').offsetHeight;
                            var curTop = document.getElementById('transcript' + i).offsetTop;
                            if (curTop > height / 2) {
                                document.getElementById('transcript-content').scrollTop = curTop - height / 2;
                            }

                        }
                        break;
                    } else {
                        $('#transcript'+i).removeClass('current');
                    }
                }

            });

        } // end attach function

    };

})(jQuery);


