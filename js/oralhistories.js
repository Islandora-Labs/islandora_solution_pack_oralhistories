/**
 * Created by Lingling Jiang on 9/16/14.
 */
(function($){
    var transcripts = [];
    //var myPlayer = videojs('islandora_videojs');
    Drupal.behaviors.islandoraOralHistories = {
        attach: function (context, settings) {
            // add transcript collapsible panel on the page
            var transcriptDispHtml = '<fieldset id="transcript-display" class="collapsible collapsed">' +
                '<legend><span class="fieldset-legend">Transcript</span></legend>' +
                '<div class="fieldset-wrapper"><div id="transcript-content"></div></div></fieldset>';
            $('.islandora-oralhistories-content', context).once(function (){
                $(this).append(transcriptDispHtml);
            });

            $('.islandora-oralhistories-content video', context).once('islandora_oralhistories').on('load', function(){

                if (Drupal.settings.islandora_oralhistories.enbableTranscriptDisplay) {
                    var enableCaption = Drupal.settings.islandora_oralhistories.enableCaptionDisplay;
                    var tracksUploaded = Drupal.settings.islandora_oralhistories.tracks;
                    var oVideo = $('.islandora-oralhistories-content video', context);
                    var newTrackHtml = '';
                    var langLabels = {
                        en: 'English',
                        fr: 'French',
                        de: 'German'
                    };
                    if (enableCaption) {
                        var trackType = 'captions';
                    } else {
                        var trackType = 'metadata';
                    }

                    for (i = 0; i<tracksUploaded.length; i++) {
                        var fileName = tracksUploaded[i]['file_name'];
                        var fnPart = fileName.split('.').shift();
                        var langCode = fnPart.substr(fnPart.length-2);
                        var srcUrl = tracksUploaded[i]['source_url'];
                        newTrackHtml += '<track id="track' + i + '" kind="' + trackType + '" ';
                        newTrackHtml += 'src="' + srcUrl + '" srclang="' + langCode + '" label="' + langLabels[langCode] + '" ';
                        if (i == 0) {
                            newTrackHtml += 'default>';
                        } else {
                            newTrackHtml += '>';
                        }
                    }

                    oVideo.append(newTrackHtml);


                    var myTextTracks = oVideo[0].textTracks;
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



                    setTimeout(loadTranscripts(), 1000);


                } //end if (enbableTranscriptDisplay)

            });



        } // end attach function

    };



    // Helper functions.
    function loadTranscripts(){
        var tTrack = $('#track0');
        for (var i=0; i < tTrack.length; i++) {
            var oTextTrack = tTrack[i].track;
            switch (oTextTrack.mode) {
                case 0:
                    oTextTrack.mode = 1;
                    break;
                case 'disabled':
                    oTextTrack.mode = 'showing';
                    break;
            }
        }


            var oCueList = tTrack[0].track.cues;
        var transHtml = '';
        for (i = 0; i < oCueList.length; i++) {
            var sText = oCueList[i].text;        // get text from first cue
            var sTime = oCueList[i].startTime;   // get start time from first cue
            var eTime = oCueList[i].endTime;
            transHtml += '<p><span id="transcript-line' + i + '" data-start-time="' + sTime + '" ';
            transHtml += 'data-end-time="' + eTime + '">' + sText + '</span></p>';
        }

        $('#transcript-content').html(transHtml);
    };




    function parseLine(l) {
        var lines = l.split("\n");
        var i = lines[0].indexOf(' --> ');
        var trans = lines[1];
        if (lines[2]) {  trans += " " + lines[2]; }
        trans = trans.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
        return {
            begin: convertSeconds(lines[0].substr(0,i)),
            btext: lines[0].substr(3,i-7),
            end: convertSeconds(lines[0].substr(i+5)),
            text: trans
        }
    };

    function convertSeconds(s) {
        var a = s.split(':');
        var r = Number(a[a.length-1]) + Number(a[a.length-2]) * 60;
        if(a.length > 2) { r+= Number(a[a.length-3]) * 3600; }
        return r;
    };

    // Interactive Transcript functions
    $('#transcript-content').on("click",function(e) {
        if(e.target.id.indexOf("transcript") == 0) {
            var i = Number(e.target.id.replace("transcript",""));
            myPlayer.currentTime(transcripts[i].begin);
            myPlayer.play();
        }
    });

})(jQuery);


