/**
 * Created by Lingling Jiang on 9/16/14.
 */
(function($){
    var transcripts = [];
    //var myPlayer = videojs('islandora_videojs');
    Drupal.behaviors.islandoraOralHistories = {
        attach: function (context, settings) {
           if (Drupal.settings.islandora_oralhistories.enbableTranscriptDisplay) {
               $('.islandora-oralhistories-content', context).append('<div id="transcript-display"><h2>Transcript</h2><div id="transcript-content"></div></div>');
               /*$('#transcript-content', context).once(function(){
                   var t = Drupal.settings.islandora_oralhistories.transcriptContent.split("\n\n");
                   console.log(t);
                   loadTranscripts();
               });*/
               var t = Drupal.settings.islandora_oralhistories.transcriptContent.split("\n\n");
               console.log(t);
               loadTranscripts();

           }
        }

    };


    // Helper functions.
    function loadTranscripts(){
        var t = Drupal.settings.islandora_oralhistories.transcriptContent.split("\n\n");
        t.shift();
        var h = "<div>";
        for(var i=0; i<t.length; i++) {
            var c = parseLine(t[i]);

            h += "<p><span id='transcript"+i+"'>"+c.text+"</span></p>";
            transcripts.push(c);
        }
        h += "</div>";
        $('#transcript-content').html(h);


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


